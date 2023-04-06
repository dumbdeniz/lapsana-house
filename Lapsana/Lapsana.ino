//  (               )   (         )     (               *      (      (      (             (               )
//  )\ )         ( /(   )\ )   ( /(     )\ )          (  `     )\ )   )\ )   )\ )          )\ )         ( /(
// (()/(   (     )\()) (()/(   )\())   (()/(    (     )\))(   (()/(  (()/(  (()/(    (    (()/(   (     )\())
//  /(_))  )\   ((_)\   /(_)) ((_)\     /(_))   )\   ((_)()\   /(_))  /(_))  /(_))   )\    /(_))  )\   ((_)/
//  _(_)  ((_)   _((_) (_))    _((_)   (_))_   ((_)  (_()((_) (_))   (_))   (_))_   ((_)  (_))   ((_)   _((_)
// |   \  | __| | \| | |_ _|  |_  /     |   \  | __| |  \/  | |_ _|  | _ \   |   \  | __| | |    | __| | \| |
// | |) | | _|  | .` |  | |    / /      | |) | | _|  | |\/| |  | |   |   /   | |) | | _|  | |__  | _|  | .` |
// |___/  |___| |_|\_| |___|  /___|     |___/  |___| |_|  |_| |___|  |_|_\   |___/  |___| |____| |___| |_|\_|
//
// Copyright (c) 2023 Deniz DEMIRDELEN
//
// MEB Bilgi ve Beceri yarışması için "Lapsana House" sketch'i

#include "LapsanaUtils.h"
#include "LapsanaWiFi.h"
#include "LapsanaSensorler.h"
#include "LapsanaConfig.h"

LapsanaWiFi wifi;
LapsanaSensorler sensorler;

//ölçülen değerleri depolamak için
float sicaklik, nem, gaz, lpg, co, duman, isik, toprakNem;

//Sensörlerin durumları
SensorDurum dhtDurumu, mq2Durumu;

unsigned long oncekiMillis = 0;

void setup() {
  //Cihazları kapalı konuma getir

  delay(100); //ESP başlangıç mesajını bekle (tamamen gereksiz, seri monitör iyi gözüksün diye)

  Serial.begin(9600);
  Serial.println("\nsetup() -----------");
    
  wifi.init(); //Wi-Fi bağlantısı kur

  sensorler.init(); //Sensörleri hazırla

  Serial.println("-------------------");
}

void loop() {
  //Veriyi şifrele
  //Ölçümleri token (api anahtarı) ile gönder
  //Geri dönen değerleri oku
  //Şifreli ise çöz
  //Değerlere göre cihazları çalıştır / Zorla çalıştırma varsa çalıştır
  
  wifi.denetle(); //loop sırasında wifi bağlantısı koparsa tekrar gelene kadar uyar
  
  const unsigned long simdikiMillis = millis();

  if (simdikiMillis - oncekiMillis >= SENSOR_INTERVAL) {
    oncekiMillis = simdikiMillis;

    sensorler.mq2Denetle(); //Gaz sensörü ısındı mı?

    //Sıcaklık ve nem için false parametre vererek yeniden deneme kapatilabilir
    dhtDurumu = sensorler.durum(Sensor::DHT11);
    sicaklik = sensorler.sicaklik(true);
    nem = sensorler.nem(true);

    //Gaz sensörünün durumunu kontrol et, eğer ısınmışsa ölçümleri al
    mq2Durumu = sensorler.durum(Sensor::MQ2);
    if (mq2Durumu == TAMAM) {
      gaz = sensorler.gaz();
      lpg = sensorler.lpg();
      co = sensorler.co();
      duman = sensorler.duman();
    }

    isik = sensorler.isik();
    toprakNem = sensorler.toprakNem();

    seriYazdir(dhtDurumu == TAMAM, mq2Durumu == TAMAM, sicaklik, nem, gaz, lpg, co, duman, isik, toprakNem);
  }
}