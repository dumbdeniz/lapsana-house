#include <Arduino.h>
#include "mbedtls\aes.h"

#include "LapsanaUtils.h" //C++ header dosyası
#include "LapsanaConfig.h" //Ayarları içeren dosya

void blink() {
  pinMode(DURUM_LED, OUTPUT);

  for(int i = 1; i <= 2; i++) {
    digitalWrite(DURUM_LED, HIGH);
    delay(100);
    digitalWrite(DURUM_LED, LOW);
    delay(100);
  }
}

void seriYazdir(bool dhtDurumu, bool mq2Durumu, float sicaklik, float nem, float gaz, float lpg, float co, float duman, float isik, float toprakNem) {
  Serial.println("\nloop() ------------");
    
  Serial.println(" DHT:");
    
  Serial.print(" - Durum    : ");
  Serial.println(dhtDurumu ? "TAMAM" : "HATA");

  if (dhtDurumu) {
    Serial.print(" - Sıcaklık : ");
    Serial.println(sicaklik);

    Serial.print(" - Nem      : ");
    Serial.println(nem);
  }
    
  Serial.println("\n MQ2:");

  Serial.print(" - Durum    : ");
  Serial.println(mq2Durumu ? "TAMAM" : "ISINIYOR");

  Serial.print(" - Gaz      : ");
  Serial.println(gaz);
    
  Serial.print(" - LPG      : ");
  Serial.println(lpg);

  Serial.print(" - CO       : ");
  Serial.println(co);

  Serial.print(" - Duman    : ");
  Serial.println(duman);

  Serial.println("\n LDR:");

  Serial.print(" - Işık     : ");
  Serial.println(isik);

  Serial.println("\n Toprak Nem:");

  Serial.print(" - Değer    : ");
  Serial.println(toprakNem);

  Serial.println("-------------------");
}

void sifrele(char* yazi) {
  unsigned char sifrelenen[16];

  //AES şifrelemeyi hazırla
  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);

  //Şifreleme anahtarını ayarla
  mbedtls_aes_setkey_enc( &aes, (const unsigned char*) AES_KEY, strlen(AES_KEY) * 8 );

  //Girilen yaziyi şifrele
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)yazi, sifrelenen);

  //AES şifreleme kaynaklarını temizle
  mbedtls_aes_free( &aes );

  Serial.println((char*)sifrelenen);
}