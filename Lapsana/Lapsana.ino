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

#include "LapsanaSensorler.h"
#include "LapsanaConfig.h"

LapsanaSensorler sensorler;

unsigned long oncekiMillis = 0;

void setup() {
  //Sensörleri hazırla - X
  //Cihazları kapalı konuma getir
  //Wi-Fi bağlantısı kur

  delay(100); //ESP başlangıç mesajını bekle (tamamen gereksiz, seri monitör iyi gözüksün diye)

  Serial.begin(9600);
  Serial.println("\nsetup() -----------");
    
  sensorler.init(); //sensörleri hazırla

  Serial.println("-------------------");
}

void loop() {
  //Bağlantı var mı kontrol et ; Bağlantı oluncaya kadar tekrar dene. Bu sırada Kırmızı led yak.
  
  //Zamanı Kontrol Et : Kaç saniyede bir bildirim yapılacak
    //Ölçümleri yap
    //Veriyi şifrele
    //Ölçümleri token (api anahtarı) ile gönder
    //Geri dönen değerleri oku
    //Şifreli ise çöz
    //Değerlere göre cihazları çalıştır / Zorla çalıştırma varsa çalıştır
  //

  const unsigned long simdikiMillis = millis();

  if (simdikiMillis - oncekiMillis >= SENSOR_INTERVAL) {
    oncekiMillis = simdikiMillis;

    sensorler.mq2Denetle(); //Gaz sensörü ısındı mı?
  }
}
