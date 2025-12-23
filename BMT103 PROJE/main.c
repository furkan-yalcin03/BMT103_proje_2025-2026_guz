#include <stdio.h>          // Standart giriş–çıkış işlemleri
#include <string.h>         // String karşılaştırma ve kopyalama işlemleri

#define MAX_ASI 100         // Maksimum aşı çeşidi sayısı
#define AD_UZUNLUK 50       // Aşı adı için ayrılan karakter uzunluğu

typedef struct {            // Aşı bilgilerini tutan yapı
    char ad[AD_UZUNLUK];    // Aşının adı
    int yapilanDoz;         // Yapılmış doz sayısı
} AsiKaydi;

int asiBul(AsiKaydi liste[], int elemanSayisi, char *arananAsi) { // Aşı listesinde arama yapar
    for (int i = 0; i < elemanSayisi; i++) {                      // Listedeki her aşıyı gezer
        if (strcmp(liste[i].ad, arananAsi) == 0) {                // Aşı ismi eşleşirse
            return i;                                             // Bulunan indeks döndürülür
        }
    }
    return -1;                                                    // Bulunamazsa -1 döner
}

int asilariOkuVeSay(char *dosyaAdi, AsiKaydi liste[]) {           // Aşıları okuyup sayar
    FILE *asilar = fopen(dosyaAdi, "r");            // Aşı kayıt dosyasını açar
    if (asilar == NULL) {                                         // Dosya açılamazsa
        printf("Hata: %s dosyasi acilamadi.\n", dosyaAdi);  // Hata mesajı verir
        return 0;                                                 // Programı sonlandırır
    }

    char okunanAsi[AD_UZUNLUK];                                   // Okunan aşı adı
    char tarih[20];                                               // Aşı tarihi (kullanılmıyor)
    int sayac = 0;                                                // Farklı aşı sayacı

    while (fscanf(asilar, "%s %s", okunanAsi, tarih) != EOF) {    // Dosya sonuna kadar okur
        int sira = asiBul(liste, sayac, okunanAsi);                     // Aşı daha önce var mı bakar

        if (sira != -1) {                                               // Daha önce eklenmişse
            liste[sira].yapilanDoz++;                                   // Doz sayısını artırır
        } else {                                                        // İlk defa karşılaşılıyorsa
            strcpy(liste[sayac].ad, okunanAsi);                         // Aşı adını kaydeder
            liste[sayac].yapilanDoz = 1;                                // İlk doz olarak ayarlar
            sayac++;                                                    // Aşı çeşidi sayısını artırır
        }
    }

    fclose(asilar);                                                // Dosyayı kapatır
    return sayac;                                                  // Toplam aşı çeşidi döndürür
}

void raporOlustur(char *zorunluDosya, char *cikisDosya,
                  AsiKaydi liste[], int listeElemanSayisi) {      // Rapor oluşturur
    FILE *zorunlu = fopen(zorunluDosya, "r");       // Zorunlu aşı dosyasını açar
    FILE *doz = fopen(cikisDosya, "w");             // Rapor dosyasını açar

    if (zorunlu == NULL || doz == NULL) {                          // Dosya hatası kontrolü
        printf("Dosya hatasi (okuma veya yazma).\n");        // Hata mesajı verir
        return;                                                    // Fonksiyondan çıkar
    }

    char zorunluAd[AD_UZUNLUK];                                    // Zorunlu aşı adı
    int zorunluDoz;                                                // Gerekli doz sayısı

    printf("\nAsiAdi Yapilan Durum\n");                      // Başlık yazdırır
    printf("------------------------\n");

    while (fscanf(zorunlu, "%s %d", zorunluAd, &zorunluDoz) != EOF) { // Zorunlu aşıları okur
        int yapilan = 0;                                                    // Yapılan doz sayısı
        int sira = asiBul(liste, listeElemanSayisi, zorunluAd);             // Aşıyı listede arar

        if (sira != -1) {                                          // Aşı yapılmışsa
            yapilan = liste[sira].yapilanDoz;                      // Yapılan doz alınır
        }

        char durum;                                                // Sonuç durumu
        if (yapilan >= zorunluDoz) durum = '+';                    // Doz yeterliyse +
        else durum = '-';                                          // Yetersizse -

        fprintf(doz, "%s %d %c\n", zorunluAd, yapilan, durum);     // Dosyaya yazar
        printf("%-10s %5d %c\n", zorunluAd, yapilan, durum);       // Ekrana yazdırır
    }

    fclose(zorunlu);                                               // Zorunlu dosyasını kapatır
    fclose(doz);                                                   // Rapor dosyasını kapatır
    printf("\nSonuclar '%s' dosyasina kaydedildi.\n", cikisDosya); // Bilgi mesajı
}

int main() {                                                       // Program başlangıcı
    AsiKaydi asiListesi[MAX_ASI];                                  // Aşı kayıt dizisi
    int toplamAsiCesidi = 0;                                       // Aşı çeşidi sayısı

    toplamAsiCesidi = asilariOkuVeSay("asilar.txt", asiListesi);   // Aşıları oku ve say

    if (toplamAsiCesidi > 0) {                                             // En az bir aşı varsa
        raporOlustur("zorunlu.txt", "dozlar.txt",
                     asiListesi, toplamAsiCesidi);                         // Rapor oluştur
    }

    return 0;                                                      // Programı sonlandır
}