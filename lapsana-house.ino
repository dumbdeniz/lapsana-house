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

#include <ESP8266WiFi.h> //Wifi kütüphanesi
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define SSID "Sseml_Sera"
#define PASS "Password1!"

#include <DHT.h> //Sıcaklık ve nem sensörü kütüphanesi - indirmek için https://cdn.demirdelen.net/dht
#define DHT_PIN D5 //Sensör pini

#include <MQ2.h> //Gaz sensörü kütüphanesi - indirmek için https://cdn.demirdelen.net/mq2

//Multiplexer pinleri
//Toprak Nem    : (C0) : S0 ve S1 LOW
//Işık Seviyesi : (C1) : S0 HIGH, S1 LOW
//Gaz Sensörü   : (C2) : S0 LOW, S1 HIGH
#define MUX_PIN A0
#define MUX_S0 D6
#define MUX_S1 D7

#define SENSOR_INTERVAL 10000 //ölçüm aralığı
#define STATUS_LED D8 //durum LED'i

#define ISTEK_URL "http://192.168.16.88/e-seracik/node.php"

DHT dht; //Sıcaklık ve nem sensörü tanımı
bool dhtDurum = false; //ölçüm başarılı ise true
float nem, sicaklik;

MQ2 mq2(MUX_PIN); //Gaz sensörü tanımı
float raw, lpg, co, duman;

int isik, toprakNem;

unsigned long oncekiMillis = 0;

//daha okunabilir bir şekilde sensorler arası geçiş için
enum sensorler { 
  DHT11, MQ2, LDR, TOPRAK_NEM
};

#pragma region Yardımcı Kodlar

//Göz kırp ;)
void blink() {
  for(int i = 1; i <= 2; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(100);
    digitalWrite(STATUS_LED, LOW);
    delay(100);
  }
}

void muxDegis(sensorler sensor) {
  int s0 = 0, s1 = 0;

  switch (sensor) {
    case LDR: s0 = 1; break; //1 - 0
    case MQ2: s1 = 1; break; //0 - 1
  }

  digitalWrite(MUX_S0, s0);
  digitalWrite(MUX_S1, s1);
  delay(100);
}

bool wifiBaglan() {
  WiFi.mode(WIFI_STA); //Modu istasyon olarak ayarla
  WiFi.begin(SSID, PASS); //Wi-Fi ağına bağlan

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  return WiFi.isConnected(); //bağlanmışsa true, herhangi bir sorun olmuşsa false
}

void seriYazdir() {
  Serial.println("\nloop() ------------");
    
  Serial.println(" DHT:");
    
  Serial.print(" - Durum    : ");
  Serial.println(dhtDurum ? "OK" : "HATA");

  if (dhtDurum) {
    Serial.print(" - Sıcaklık : ");
    Serial.println(sicaklik);

    Serial.print(" - Nem      : ");
    Serial.println(nem);
  }
    
  Serial.println("\n MQ2:");

  Serial.print(" - RAW      : ");
  Serial.println(raw);
    
  Serial.print(" - LPG      : ");
  Serial.println(lpg);

  Serial.print(" - CO       : ");
  Serial.println(co);

  Serial.print(" - Duman    : ");
  Serial.println(duman);

  Serial.println("\n LDR:");

  Serial.print(" - Işık     : ");
  Serial.println(isik);

  Serial.println("\n T. Nem:");

  Serial.print(" - Değer    : ");
  Serial.println(toprakNem);

  Serial.println("-------------------");
}

#pragma endregion

void setup() {
  //Sensörleri hazırla
  //Cihazları kapalı konuma getir
  //Wi-Fi bağlantısı kur

  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);

  delay(100); //ESP başlangıç mesajını bekle (tamamen gereksiz, seri monitör iyi gözüksün diye)

  Serial.begin(9600);
  Serial.println("\nsetup() -----------");

  Serial.print("SSID : ");
  Serial.println(SSID);

  bool wifiSonuc = wifiBaglan();
  if (!wifiSonuc) {
    Serial.println("Başarısız");

    //reset atılana kadar uyar
    while(true) {
      blink();
      delay(1000);
    }
  }
  
  Serial.println("Bağlandı");

  Serial.print("IP : ");
  Serial.println(WiFi.localIP()); 
    
  dht.setup(DHT_PIN, DHT::DHT11); //DHT11'i hazırla

  muxDegis(MQ2); //gaz sensörüne geç
  mq2.begin(); //sensörü hazırla ve kalibre et

  Serial.println("-------------------");
}

void loop() {
  //Bağlantı var mı kontrol et ; Bağlantı oluncaya kadar tekrar dene. Bu sırada Kırmızı led yak.
  
  //Zamanı Kontrol Et : Kaç saniyede bir bildirim yapılacak
    //Ölçümleri yap
      //havaSicaklikVeNem_Olc();
    //Veriyi şifrele
    //Ölçümleri token (api anahtarı) ile gönder
    //Geri dönen değerleri oku
    //Şifreli ise çöz
    //Değerlere göre cihazları çalıştır / Zorla çalıştırma varsa çalıştır
  //

  //loop sırasında wifi bağlantısı koparsa tekrar gelene kadar uyar
  //ESP kayıtlı bir ağın bağlantısı koparsa kendisi otomatik geri bağlanır, bu olana kadar uyaralım.
  while(!WiFi.isConnected()) {
    blink();
    delay(1000);
  }

  const unsigned long simdikiMillis = millis();

  //her ölçüm aralığında
  if (simdikiMillis - oncekiMillis >= SENSOR_INTERVAL) {
    oncekiMillis = simdikiMillis;

    

    sicaklikVeNem();
    gazSeviyesi();
    isikSeviyesi();
    toprakNemSeviyesi();

    //seriYazdir();

    String istek = SUNUCU_ADI;
    http.begin(client, istek.c_str()); 
    int responseCode = http.GET();
    Serial.println(responseCode, 1);
    Serial.println(http.getString());
  }
}

#pragma region Veri Gönderme

bool veriGonder() {
  WiFiClient client;
  HTTPClient http;

  String istek = ISTEK_URL + 
}

#pragma endregion

#pragma region Sensor Olcümleri

void sicaklikVeNem() {
  dhtDurum = dht.getStatusString() == "OK";
  
  nem = dhtDurum ? dht.getHumidity() : 0;
  sicaklik = dhtDurum ? dht.getTemperature() : 0;  
}

void gazSeviyesi() {
  muxDegis(MQ2);

  raw = analogRead(MUX_PIN);
  lpg = mq2.readLPG();
  co = mq2.readCO();
  duman = mq2.readSmoke();
}

void isikSeviyesi() {
  muxDegis(LDR);

  isik = analogRead(MUX_PIN);
}

void toprakNemSeviyesi() {
  muxDegis(TOPRAK_NEM);

  toprakNem = analogRead(MUX_PIN);
}

#pragma endregion
