#include <string>


struct CompressOptions {
    int  videoBitrate  = 1000000;  // 1 Mbps
    int  audioBitrate  = 128000;   // 128 kbps
    int  crf           = 23;       // Kalite faktörü (0-51, düşük = daha iyi)
    std::string preset = "medium"; // ultrafast, fast, medium, slow, veryslow
    std::string codec  = "libx264";
};


void compressVideo(const std::string& inputPath,
                   const std::string& outputPath,
                   const CompressOptions& opts = {});