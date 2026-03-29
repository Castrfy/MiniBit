# 🛰️ MiniBit — Veri Sıkıştırma Paneli

Uzay araçlarına aktarım için veri sıkıştırma işlemi yapan bir uygulama. Python ve CustomTkinter ile oluşturuldu.

---

## Önizleme

MiniBit, binlerce kilometre uzaklıktaki uzay araçlarına veri göndermek için kullanımı kolay bir arayüz sunar. Dosya yüklenir veya elle girilir, sıkıştırma yöntemi seçilir, sonuç görüntülenir ve bir sunucu adresi üzerinden araca iletilir.

---

## Özellikler

- 📂 **Dosya yükleme** — cihazdan doğrudan `.txt` dosyalarını yüklemek
- ✏️ **Elle giriş** — metin yazıp yapıştırabilmek
- ⚙️ **Birçok sıkıştırma seçeneği** — durum gereksinimlerine göre en uygun yöntemi seçebilmek
- 📡 **Uzay aracına iletim** — API sunucu adresi girip sıkıştırılmış verileri göndermek

---

## Başlangıç

### Gereklilikler

- Python 3.8+
- \`customtkinter\`

\`\`\`bash
pip install customtkinter
\`\`\`

### Uygulamayı Çalıştırma

\`\`\`bash
python main.py
\`\`\`

> \`icon.ico\` dosyasının \`main.py\` ile aynı dizinde olduğundan emin olun.

---

## Kullanım

1. **Veri yükleme** — *Browse File* alanını kullanarak \`.txt\` dosyası yüklenebilir veya elle veri girilebilir
2. **Sunucu adresi girme** — uzay aracının API adresi girilir ve *Set API* tuşuna basılır
3. **Sıkıştırma yöntemi seçme** — dropdown kutusundan istenen yöntem seçilir
4. ***Compress* butonuna bas** — sıkıştırılan veri sonuç ekranında görünür
5. ***Deliver To Spacecraft* tuşunu kullan** — sıkıştırılan veriyi uzay aracına gönderir

---

## Proje Yapısı

\`\`\`
MiniBit/
├── main.py        # Ana uygulama
├── icon.ico       # Uygulama ikonu
└── README.md
\`\`\`

---

## Yol Haritası

- [ ] Gerçek sıkıştırma algoritmaları entegre etmek
- [ ] Sunucu transfer geri bildirimi eklemek (başarı/hata yanıtı)
- [ ] Farklı tür dosyaları desteklemek (\`.csv\`, \`.json\`, \`.bin\`)
- [ ] Sıkıştırma bilgilerini gösteren ekran oluşturmak
- [ ] Gönderi geçmişi

---

## Katkılar

Pull request'lere açığız. Büyük değişiklikler için önce bir issue açarak ne değiştirmek istediğinizi tartışın.

---

## Lisans

[MIT](LICENSE)
EOF
