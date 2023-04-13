//  _                                                       _   _                              
// | |       __ _   _ __    ___    __ _   _ __     __ _    | | | |   ___    _   _   ___    ___ 
// | |      / _` | | '_ \  / __|  / _` | | '_ \   / _` |   | |_| |  / _ \  | | | | / __|  / _ \
// | |___  | (_| | | |_) | \__ \ | (_| | | | | | | (_| |   |  _  | | (_) | | |_| | \__ \ |  __/
// |_____|  \__,_| | .__/  |___/  \__,_| |_| |_|  \__,_|   |_| |_|  \___/   \__,_| |___/  \___|
//                 |_|                                                                         
//
// Sedat Simavi Endüstri Meslek Lisesi • MEB Bilgi ve Beceri yarışması için "Lapsana House" sketch'i

#include "LapsanaSensorler.h"
#include "LapsanaCihazlar.h"
#include "LapsanaUtils.h"
#include "LapsanaWiFi.h"
#include "LapsanaConfig.h"

LapsanaWiFi wifi;
LapsanaSensorler sensorler;
LapsanaCihazlar cihazlar;

SensorDegerler degerler;
SensorDurumlar durumlar;
CihazDurumlar cihazDurumlar;

unsigned long oncekiMillis = 0;

void setup() {
  delay(100); //ESP başlangıç mesajını bekle

  Serial.begin(9600);
  Serial.println("\nsetup() -----------");
  
  wifi.init(); //Wi-Fi bağlantısı kur

  sensorler.init(); //Sensörleri hazırla

  cihazlar.init(); //Cihazları hazırla

  Serial.println("-------------------");
}

void loop() {
  wifi.denetle(); //loop sırasında wifi bağlantısı koparsa tekrar gelene kadar uyar
  
  const unsigned long simdikiMillis = millis();

  //Sensor ölçüm aralığı geçti mi?
  if (simdikiMillis - oncekiMillis >= SENSOR_ARALIK) {
    oncekiMillis = simdikiMillis;
    
    sensorler.hallet(degerler, durumlar); //Ölçümleri al ve değişkenlerde depola
    
    sensorlerYazdir(degerler, durumlar); //Hata ayıklama için yazdır
    
    wifi.httpsGonder(degerler, durumlar, cihazDurumlar); //Verileri şifreleyip gönder, yanıtı değişkende depola
    
    cihazlar.hallet(cihazDurumlar); //Gelen yanıta göre cihazları açıp kapat

    cihazlarYazdir(cihazDurumlar); //Hata ayıklama için yazdır
  }
}