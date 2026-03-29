#include <iostream>
#include <string>
#include <stdexcept>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
}

struct CompressOptions {
    int  videoBitrate  = 1000000;  // 1 Mbps
    int  audioBitrate  = 128000;   // 128 kbps
    int  crf           = 23;       // Kalite faktörü (0-51, düşük = daha iyi)
    std::string preset = "medium"; // ultrafast, fast, medium, slow, veryslow
    std::string codec  = "libx264";
};

// -----------------------------------------------------------------------
// Yardımcı: hata kontrolü
// -----------------------------------------------------------------------
static void checkFFmpeg(int ret, const std::string& msg) {
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE]{};
        av_strerror(ret, errbuf, sizeof(errbuf));
        throw std::runtime_error(msg + ": " + errbuf);
    }
}

// -----------------------------------------------------------------------
// Ana fonksiyon
//   inputPath  : kaynak video dosyası
//   outputPath : sıkıştırılmış çıktı dosyası
//   opts       : sıkıştırma parametreleri
// -----------------------------------------------------------------------
void compressVideo(const std::string& inputPath,
                   const std::string& outputPath,
                   const CompressOptions& opts = {})
{
    // ---------- 1. Giriş dosyasını aç ----------
    AVFormatContext* inFmtCtx = nullptr;
    checkFFmpeg(avformat_open_input(&inFmtCtx, inputPath.c_str(), nullptr, nullptr),
                "Giriş dosyası açılamadı");
    checkFFmpeg(avformat_find_stream_info(inFmtCtx, nullptr),
                "Akış bilgisi bulunamadı");

    // ---------- 2. Çıkış bağlamını oluştur ----------
    AVFormatContext* outFmtCtx = nullptr;
    checkFFmpeg(avformat_alloc_output_context2(&outFmtCtx, nullptr, nullptr, outputPath.c_str()),
                "Çıkış bağlamı oluşturulamadı");

    // ---------- 3. Her akış için encoder/decoder kur ----------
    // Video encoder
    const AVCodec* videoEncoder = avcodec_find_encoder_by_name(opts.codec.c_str());
    if (!videoEncoder)
        throw std::runtime_error("Video encoder bulunamadı: " + opts.codec);

    AVCodecContext* videoEncCtx = nullptr;
    AVCodecContext* videoDecCtx = nullptr;
    int videoStreamIdx = -1;
    int audioStreamIdx = -1;

    // Akışları eşle
    for (unsigned i = 0; i < inFmtCtx->nb_streams; ++i) {
        AVStream* inStream  = inFmtCtx->streams[i];
        AVStream* outStream = avformat_new_stream(outFmtCtx, nullptr);
        if (!outStream)
            throw std::runtime_error("Çıkış akışı oluşturulamadı");

        if (inStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIdx = (int)i;

            // Decoder
            const AVCodec* dec = avcodec_find_decoder(inStream->codecpar->codec_id);
            videoDecCtx = avcodec_alloc_context3(dec);
            checkFFmpeg(avcodec_parameters_to_context(videoDecCtx, inStream->codecpar),
                        "Decoder parametreleri kopyalanamadı");
            checkFFmpeg(avcodec_open2(videoDecCtx, dec, nullptr),
                        "Decoder açılamadı");

            // Encoder
            videoEncCtx = avcodec_alloc_context3(videoEncoder);
            videoEncCtx->width       = videoDecCtx->width;
            videoEncCtx->height      = videoDecCtx->height;
            videoEncCtx->sample_aspect_ratio = videoDecCtx->sample_aspect_ratio;
            videoEncCtx->pix_fmt     = videoEncoder->pix_fmts
                                       ? videoEncoder->pix_fmts[0]
                                       : videoDecCtx->pix_fmt;
            videoEncCtx->time_base   = inStream->r_frame_rate.num
                                       ? AVRational{inStream->r_frame_rate.den,
                                                    inStream->r_frame_rate.num}
                                       : AVRational{1, 25};
            videoEncCtx->bit_rate    = opts.videoBitrate;
            videoEncCtx->framerate   = inStream->r_frame_rate;

            av_opt_set(videoEncCtx->priv_data, "preset", opts.preset.c_str(), 0);
            av_opt_set_int(videoEncCtx->priv_data, "crf", opts.crf, 0);

            if (outFmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
                videoEncCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

            checkFFmpeg(avcodec_open2(videoEncCtx, videoEncoder, nullptr),
                        "Video encoder açılamadı");
            checkFFmpeg(avcodec_parameters_from_context(outStream->codecpar, videoEncCtx),
                        "Encoder parametreleri kopyalanamadı");
            outStream->time_base = videoEncCtx->time_base;

        } else if (inStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIdx = (int)i;
            // Ses akışını olduğu gibi kopyala (stream copy)
            checkFFmpeg(avcodec_parameters_copy(outStream->codecpar, inStream->codecpar),
                        "Ses parametreleri kopyalanamadı");
            outStream->codecpar->bit_rate = opts.audioBitrate;
            outStream->time_base = inStream->time_base;
        } else {
            // Diğer akışları (altyazı vb.) doğrudan kopyala
            checkFFmpeg(avcodec_parameters_copy(outStream->codecpar, inStream->codecpar),
                        "Akış parametreleri kopyalanamadı");
            outStream->time_base = inStream->time_base;
        }
    }

    // ---------- 4. Çıkış dosyasını aç ----------
    if (!(outFmtCtx->oformat->flags & AVFMT_NOFILE))
        checkFFmpeg(avio_open(&outFmtCtx->pb, outputPath.c_str(), AVIO_FLAG_WRITE),
                    "Çıkış dosyası açılamadı");

    checkFFmpeg(avformat_write_header(outFmtCtx, nullptr), "Header yazılamadı");

    // ---------- 5. Paketleri oku / yeniden encode et ----------
    AVPacket* pkt   = av_packet_alloc();
    AVFrame*  frame = av_frame_alloc();
    AVFrame*  encFrame = av_frame_alloc();

    // Piksel format dönüşümü için (gerekirse)
    SwsContext* swsCtx = nullptr;

    while (av_read_frame(inFmtCtx, pkt) >= 0) {
        AVStream* inStream  = inFmtCtx->streams[pkt->stream_index];
        AVStream* outStream = outFmtCtx->streams[pkt->stream_index];

        if (pkt->stream_index == videoStreamIdx) {
            // Video: decode → encode
            checkFFmpeg(avcodec_send_packet(videoDecCtx, pkt), "Decode paketi gönderilemedi");

            while (avcodec_receive_frame(videoDecCtx, frame) == 0) {
                // Format dönüşümü
                if (!swsCtx) {
                    swsCtx = sws_getContext(
                        frame->width, frame->height, (AVPixelFormat)frame->format,
                        videoEncCtx->width, videoEncCtx->height, videoEncCtx->pix_fmt,
                        SWS_BICUBIC, nullptr, nullptr, nullptr);
                }

                encFrame->width  = videoEncCtx->width;
                encFrame->height = videoEncCtx->height;
                encFrame->format = videoEncCtx->pix_fmt;
                av_frame_get_buffer(encFrame, 0);

                sws_scale(swsCtx,
                          frame->data, frame->linesize, 0, frame->height,
                          encFrame->data, encFrame->linesize);

                encFrame->pts = frame->pts;

                // Encode
                checkFFmpeg(avcodec_send_frame(videoEncCtx, encFrame),
                            "Encode frame gönderilemedi");

                AVPacket* encPkt = av_packet_alloc();
                while (avcodec_receive_packet(videoEncCtx, encPkt) == 0) {
                    av_packet_rescale_ts(encPkt, videoEncCtx->time_base, outStream->time_base);
                    encPkt->stream_index = pkt->stream_index;
                    av_interleaved_write_frame(outFmtCtx, encPkt);
                }
                av_packet_free(&encPkt);
                av_frame_unref(encFrame);
            }
        } else {
            // Ses / diğer: doğrudan kopyala
            av_packet_rescale_ts(pkt, inStream->time_base, outStream->time_base);
            av_interleaved_write_frame(outFmtCtx, pkt);
        }

        av_packet_unref(pkt);
    }

    // ---------- 6. Encoder'ı boşalt (flush) ----------
    avcodec_send_frame(videoEncCtx, nullptr);
    AVPacket* flushPkt = av_packet_alloc();
    while (avcodec_receive_packet(videoEncCtx, flushPkt) == 0) {
        AVStream* outStream = outFmtCtx->streams[videoStreamIdx];
        av_packet_rescale_ts(flushPkt, videoEncCtx->time_base, outStream->time_base);
        flushPkt->stream_index = videoStreamIdx;
        av_interleaved_write_frame(outFmtCtx, flushPkt);
    }
    av_packet_free(&flushPkt);

    // ---------- 7. Temizlik ----------
    av_write_trailer(outFmtCtx);

    sws_freeContext(swsCtx);
    av_frame_free(&frame);
    av_frame_free(&encFrame);
    av_packet_free(&pkt);
    avcodec_free_context(&videoDecCtx);
    avcodec_free_context(&videoEncCtx);
    avformat_close_input(&inFmtCtx);
    if (!(outFmtCtx->oformat->flags & AVFMT_NOFILE))
        avio_closep(&outFmtCtx->pb);
    avformat_free_context(outFmtCtx);

    std::cout << "Sıkıştırma tamamlandı: " << outputPath << "\n";
}

// -----------------------------------------------------------------------
// Örnek kullanım
// -----------------------------------------------------------------------
int main() {
    CompressOptions opts;
    opts.videoBitrate = 800000;   // 800 kbps
    opts.crf          = 28;       // Daha fazla sıkıştırma
    opts.preset       = "fast";
    opts.codec        = "libx264";

    try {
        compressVideo("input.mp4", "output_compressed.mp4", opts);
    } catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << "\n";
        return 1;
    }
    return 0;
}