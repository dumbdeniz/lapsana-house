#include <Arduino.h>
#include "LapsanaSensorler.h"

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

  Serial.println("-------------------");
}