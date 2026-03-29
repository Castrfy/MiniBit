# 🛰️ MiniBit — Veri Sıkıştırma Paneli

Uzay araçlarına aktarım için veri sıkıştırma işlemi yapan bir uygulama. Python'ın CustomTkinteri ve C++ ile oluşturuldu.

---

## Önizleme

MiniBit'in amacı, binlerce kilometre uzaklıktaki uzay araçlarına veri göndermek için kullanımı kolay bir arayüz sunmaktır. Dosya yüklenir veya elle girilir, sıkıştırma yöntemi seçilir, sonuç görüntülenir ve bir sunucu adresi üzerinden araca iletilir.

---

## Özellikler

- 📂 **Dosya yükleme** — cihazdan doğrudan her türden dosya yüklemek
- ⚙️ **Birçok sıkıştırma seçeneği** — durum gereksinimlerine göre en uygun yöntemi seçebilmek
- 📡 **Uzay aracına iletim** — API sunucu adresi girip sıkıştırılmış verileri göndermek

---

## Başlangıç

### Gereklilikler

- Python 3.8+
- `customtkinter`
- CMake

```bash
pip install customtkinter
```

`/cpp/` klasörünün içinde build adlı bir klasör oluşturun ve içine girin ardından,
```bash
cmake ..
make
```
komutlarını çalıştırarak cpp dosyalarını derleyin

### Uygulamayı Derleme

```bash
python -m PyInstaller satellite_panel.py --onefile --noconsole
```

> `icon.ico` dosyasının `satellite_panel.exe` ile aynı dizinde olduğundan emin olun.
> derlenmiş haldeki `minibit_compresser`dosyasının `satellite_panel.exe` ile aynı dizindeki `cpp/build/` klasörünün içinde olduğuna emin olun

sonrasında uygulamayı çalıştırabilirsiniz.

### Ya da direk olarak kodu çalıştırın

```bash
python satellite_panel.py
```

> `icon.ico` dosyasının `satellite_panel.py` ile aynı dizinde olduğundan emin olun.
> derlenmiş haldeki `minibit_compresser` dosyasının `satellite_panel.py` ile aynı dizindeki `cpp/build/` klasörünün içinde olduğuna emin olun

---

## Kullanım

1. **Veri yükleme** — *Browse File* alanını kullanarak `.txt` dosyası yüklenebilir veya elle veri girilebilir
2. **Sunucu adresi girme** — uzay aracının API adresi girilir ve *Set API* tuşuna basılır
3. **Sıkıştırma yöntemi seçme** — dropdown kutusundan istenen yöntem seçilir
4. ***Compress* butonuna bas** — sıkıştırılan veri sonuç ekranında görünür
5. ***Deliver To Spacecraft* tuşunu kullan** — sıkıştırılan veriyi uzay aracına gönderir

---

## Proje Yapısı

```
MiniBit/
├──cpp
│   └──build
│        └── minibit_compresser # Binary çalıştırılabilir uygulama
├── main.py        # Ana uygulama
└── icon.ico       # Uygulama ikonu 
```

---

## Yol Haritası

- [√] Gerçek sıkıştırma algoritmaları entegre etmek
- [√] Farklı tür dosyaları desteklemek (`.csv`, `.json`, `.bin`)
- [ ] Daha fazla sıkıştırma algoritması ekleme
- [ ] Dosyalara ihtiyaç duymadan elle mesaj yazılıp sıkıştırabilme ve sunucuya gönderme
- [ ] Sunucu transfer geri bildirimi eklemek (başarı/hata yanıtı)
- [ ] Sıkıştırma bilgilerini gösteren ekran oluşturmak
- [ ] Gönderi geçmişi

---

## Katkılar

Pull request'lere açığız. Büyük değişiklikler için önce bir issue açarak ne değiştirmek istediğinizi tartışın.

---

## Lisans

[MIT](LICENSE)
EOF
