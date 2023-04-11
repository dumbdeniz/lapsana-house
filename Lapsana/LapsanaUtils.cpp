#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "LapsanaSensorler.h"

#include "Base64.h"
#include "LapsanaUtils.h" //C++ header dosyası
#include "LapsanaConfig.h" //Ayarları içeren dosya

//u8x/A?D(G+KbPeSh
uint8_t sifreleme_iv[16] {0x75, 0x38, 0x78, 0x2F, 0x41, 0x3F, 0x44, 0x28, 0x47, 0x2B, 0x4B, 0x62, 0x50, 0x65, 0x53, 0x68};

void blink() {
  pinMode(DURUM_LED, OUTPUT);

  for(int i = 1; i <= 2; i++) {
    digitalWrite(DURUM_LED, HIGH);
    delay(100);
    digitalWrite(DURUM_LED, LOW);
    delay(100);
  }
}

void json(char* cikis, const SensorDegerler &degerler, const SensorDurumlar &durumlar) {
  char jsonBuffer[256];
  
  int sicaklik = durumlar.dht11 == TAMAM ? static_cast<int>(degerler.sicaklik) : -1;
  int nem = durumlar.dht11 == TAMAM ? static_cast<int>(degerler.nem) : -1;
  int gaz = durumlar.mq2 == TAMAM ? static_cast<int>(degerler.gaz) : -1;
  int isik = durumlar.ldr == TAMAM ? static_cast<int>(degerler.gaz) : -1;
  int toprakNem = durumlar.toprakNem == TAMAM ? static_cast<int>(degerler.toprakNem) : -1;
  auto suSeviyesi = degerler.suSeviyesi ? "true" : "false";

  //Örnek veriler (daha cihazlar eklenmedi)
  auto isitici = "false";
  auto lamba = "true";
  auto pompa = "true";
  auto fan = "false";
  auto pencere = "false";

  sprintf(jsonBuffer, JSON_FORMAT, 
    sicaklik, nem, gaz, isik, toprakNem, suSeviyesi, 
      durumlar.dht11, durumlar.mq2, durumlar.ldr, durumlar.toprakNem,
        isitici, lamba, pompa, fan, pencere
  );

  for(int i=0; i < 256; ++i)
    cikis[i] = jsonBuffer[i];
}

void sifrele(char giris[], char* cikis) {
  int uzunluk = strlen(giris);
  int bloklar = uzunluk / 16 + 1;
  uint8_t padding = bloklar * 16 - uzunluk;
  uint8_t veri[bloklar*16];
  uint8_t iv[16];

  //Girilen değeri bloklara uygun olarak kopyala
  memcpy(veri, giris, uzunluk);
  memcpy(iv, sifreleme_iv, 16);

  //Geri kalanı boşluklarla doldur
  for (int i = uzunluk; i < bloklar * 16; i++) veri[i] = padding;

  //Şifreleme context'ini ayarla, 32 byte'lık anahtar ile hazırla ve veriyi statik IV ile şifrele
  br_aes_big_cbcenc_keys encCtx;
  br_aes_big_cbcenc_init(&encCtx, AES_KEY, 32);
  br_aes_big_cbcenc_run(&encCtx, iv, veri, bloklar * 16);

  //Base64 olarak kodla
  uzunluk = bloklar * 16;
  char sifrelenmis_veri[base64_enc_len(uzunluk)];
  base64_encode(sifrelenmis_veri, (char*)veri, uzunluk);
  
  //Çıkışa yaz
  for(int i = 0; i <= 320; i++) cikis[i] = sifrelenmis_veri[i];
}

void sifreCoz(char giris[], char* cikis) {
  int girisUzunluk = strlen(giris);
  int uzunluk = base64_dec_len(giris, girisUzunluk);
  int bloklar = uzunluk / 16;
  uint8_t veri[uzunluk];
  uint8_t iv[16];

  //Base64 kodunu çöz
  base64_decode((char *)veri, giris, girisUzunluk);

  memcpy(iv, sifreleme_iv, 16);

  //Şifre çözme context'ini ayarla, anahtar ve IV'yi hazırla ve şifreyi çöz
  br_aes_big_cbcdec_keys decCtx;
  br_aes_big_cbcdec_init(&decCtx, AES_KEY, 32);
  br_aes_big_cbcdec_run(&decCtx, iv, veri, bloklar *16);

  //boşluk ve blok boyularını ayarla
  uint8_t padding = veri[bloklar * 16 -1];
  uzunluk = bloklar * 16 - padding;

  //çözülmüş veriyi kopyala, sonuna null karakteri ekle
  char cozulmus_veri[uzunluk + 1];
  memcpy(cozulmus_veri, veri, uzunluk);
  cozulmus_veri[uzunluk] = '\0';

  for(int i = 0; i <= uzunluk; i++) cikis[i] = cozulmus_veri[i];
}

void seriYazdir(SensorDegerler degerler, SensorDurumlar durumlar) {
  Serial.println("\nloop() ------------");
    
  Serial.println(" DHT:");
    
  Serial.print(" - Durum    : ");
  Serial.println(durumlar.dht11 == TAMAM ? "TAMAM" : "HATA");

  if (durumlar.dht11 == TAMAM) {
    Serial.print(" - Sıcaklık : ");
    Serial.println(degerler.sicaklik);

    Serial.print(" - Nem      : ");
    Serial.println(degerler.nem);
  }
    
  Serial.println("\n MQ2:");

  Serial.print(" - Durum    : ");
  Serial.println(durumlar.mq2 == TAMAM ? "TAMAM" : "ISINIYOR");

  if (durumlar.mq2 == 1) {
    Serial.print(" - Gaz      : ");
    Serial.println(degerler.gaz);
  }

  Serial.println("\n LDR:");  
  
  Serial.print(" - Durum    : ");
  Serial.println(durumlar.ldr == TAMAM ? "TAMAM" : "HATA");

  Serial.print(" - Işık     : ");
  Serial.println(degerler.isik);

  Serial.println("\n Toprak Nem:");

  Serial.print(" - Durum    : ");
  Serial.println(durumlar.toprakNem == TAMAM ? "TAMAM" : "HATA");

  Serial.print(" - Değer    : ");
  Serial.println(degerler.toprakNem);

  Serial.println("\n Su Seviyesi:");

  Serial.print(" - Değer    : ");
  Serial.println(degerler.suSeviyesi == 1 ? "true" : "false");
}