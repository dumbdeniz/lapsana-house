//  _                                                       _   _                              
// | |       __ _   _ __    ___    __ _   _ __     __ _    | | | |   ___    ___   _   _    ___ 
// | |      / _` | | '_ \  / __|  / _` | | '_ \   / _` |   | |_| |  / _ \  / __| | | | |  / _ \
// | |___  | (_| | | |_) | \__ \ | (_| | | | | | | (_| |   |  _  | | (_) | \__ \ | |_| | |  __/
// |_____|  \__,_| | .__/  |___/  \__,_| |_| |_|  \__,_|   |_| |_|  \___/  |___/  \__,_|  \___|
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
  //ESP başlangıç mesajını bekle
  delay(100); 

  Serial.begin(9600);
  Serial.println("\nsetup() -----------");
  
  //Wi-Fi bağlantısı kur
  wifi.init(); 

  //Sensörleri hazırla
  sensorler.init(); 

  //Cihazları hazırla
  cihazlar.init(); 

  Serial.println("-------------------");
}

void loop() {
  //loop sırasında wifi bağlantısı koparsa tekrar gelene kadar uyar
  wifi.denetle();
  
  const unsigned long simdikiMillis = millis();

  //Sensor ölçüm aralığı geçti mi?
  if (simdikiMillis - oncekiMillis >= SENSOR_ARALIK) {
    oncekiMillis = simdikiMillis;

    //Ölçümleri al ve değişkenlerde depola
    sensorler.hallet(degerler, durumlar);

    //Hata ayıklama için yazdır
    sensorlerYazdir(degerler, durumlar);

    //Verileri şifreleyip gönder, yanıtı değişkende depola
    wifi.httpsGonder(degerler, durumlar, cihazDurumlar);

    //Gelen yanıta göre cihazları açıp kapat
    cihazlar.hallet(cihazDurumlar);

    //Hata ayıklama için yazdır
    cihazlarYazdir(cihazDurumlar);
  }
}