#include <Arduino.h> //Arduino kütüphanesi
#include <DHT.h> //Sıcaklık ve nem sensörü kütüphanesi - indirmek için https://cdn.demirdelen.net/dht
#include <MQ2.h> //Gaz sensörü kütüphanesi - indirmek için https://cdn.demirdelen.net/mq2

#include "LapsanaSensorler.h" //C++ header dosyası
#include "LapsanaConfig.h" //Ayarları içeren dosya

DHT dht; //Sıcaklık ve nem sensörü tanımı
MQ2 mq2(MUX_PIN); //Gaz sensörü tanımı

//Sensör durumları, hata var mı, yok mu vb. kontrol etmek için;
SensorDurum dhtDurum = TAMAM, mq2Durum = ISINIYOR, ldrDurum = TAMAM, toprakNemDurum = TAMAM, suSeviyeDurum = TAMAM;

#pragma region Public Kodlar

void LapsanaSensorler::init() {
  //Multiplexer pinlerini ayarla
  pinMode(MUX_MQ2, OUTPUT);
  pinMode(MUX_LDR, OUTPUT);
  pinMode(MUX_LDR2, OUTPUT);
  pinMode(MUX_TNEM, OUTPUT);

  //DHT11'i hazırla
  dht.setup(DHT_PIN, DHT::DHT11);

  //DHT sensör durumunu kontrol et
  dhtDurumYenidenDene();

  //Gaz sensörünün ısınma süresini atlama pini bağlanmışsa geç ve sensörü hazırla, değilse ısınıyor olarak ayarla
  //Isınma süresi içinde herhangi bir ölçüm isteği gelirse sensör ısınana kadar veri göndermez
  if (digitalRead(MQ2_ISINMA_ATLAMA) || MQ2_ISINMA_SURESI == 0) {
    mq2Durum = TAMAM;
    mux(Sensor::MQ2); //sensöre geç
    mq2.begin();
  } else {
    mq2Durum = ISINIYOR;
  }
}

void LapsanaSensorler::mq2Denetle() {
  if (mq2Durum == ISINIYOR) {
    unsigned long isinmaSuresi = MQ2_ISINMA_SURESI * 60 * 1000; //dakika * saniye * milisaniye

    //Isınma süresi geçtiyse kalibrasyona başla ve sensörü hazırla
    if (millis() >= isinmaSuresi) { 
      mq2.begin();
      mq2Durum = TAMAM;
    }
  }
}

float LapsanaSensorler::sicaklik(bool yenidenDene) {
  if (dhtDurum != TAMAM && yenidenDene) {
    dhtDurumYenidenDene();
    if (dhtDurum != TAMAM) return -1;
  }

  float sicaklik = dht.getTemperature();

  //ölçüm başarısızsa tekrar dene
  if (isnan(sicaklik) && yenidenDene) {
    dhtOlcumYenidenDene(&sicaklik, true);
    if (isnan(sicaklik)) return -1;
  }

  return sicaklik;
}

float LapsanaSensorler::nem(bool yenidenDene) {
  if (dhtDurum != TAMAM && yenidenDene) {
    dhtDurumYenidenDene();
    if (dhtDurum != TAMAM) return -1;
  }

  float nem = dht.getHumidity();

  //ölçüm başarısızsa tekrar dene
  if (isnan(nem) && yenidenDene) {
    dhtOlcumYenidenDene(&nem, false);
    if (isnan(nem)) return -1;
  }

  return nem;
}

float LapsanaSensorler::gaz() {
  if (mq2Durum != TAMAM) return -1;

  mux(Sensor::MQ2);

  float olcumler = 0;
  olcumler += analogOrnekle();

  return olcumler / ANALOG_ORNEK_SAYISI;
}

float LapsanaSensorler::lpg() {
  mux(Sensor::MQ2);

  return mq2Durum == TAMAM ? mq2.readLPG() : -1;
}

float LapsanaSensorler::co() {
  mux(Sensor::MQ2);

  return mq2Durum == TAMAM ? mq2.readCO() : -1;
}

float LapsanaSensorler::duman() {
  mux(Sensor::MQ2);

  return mq2Durum == TAMAM ? mq2.readSmoke() : -1;
}

float LapsanaSensorler::isik() {
  float olcumler = 0.0;

  mux(Sensor::LDR); //ilk LDR sensörüne geç

  olcumler += analogOrnekle();

  mux(Sensor::LDR2); //ikinci LDR sensörüne geç

  olcumler += analogOrnekle();

  return olcumler / (ANALOG_ORNEK_SAYISI * 2);
}

float LapsanaSensorler::toprakNem() {
  mux(Sensor::TOPRAK_NEM);

  float olcumler = 0.0;
  olcumler += analogOrnekle();

  return olcumler / ANALOG_ORNEK_SAYISI;
}

bool LapsanaSensorler::suSeviyesi() {
  return digitalRead(SU_SEVIYE_PIN);
}

SensorDurum LapsanaSensorler::durum(Sensor sensor) {
  SensorDurum durum;

  switch(sensor) {
    case Sensor::DHT11: durum = dhtDurum; break;
    case Sensor::MQ2: durum = mq2Durum; break;
    case Sensor::LDR: durum = ldrDurum; break;
    case Sensor::TOPRAK_NEM: durum = toprakNemDurum; break;
    case Sensor::SU_SEVIYE: durum = suSeviyeDurum; break;
  }

  return durum;
}

#pragma endregion

#pragma region Private Kodlar

void LapsanaSensorler::mux(Sensor sensor) {
  int d1 = 0, d2 = 0, d3 = 0, d4 = 0;

  switch (sensor) {
    case Sensor::MQ2: d1 = 1; break;
    case Sensor::LDR: d2 = 1; break;
    case Sensor::LDR2: d3 = 1; break;
    case Sensor::TOPRAK_NEM: d4 = 1; break;
  }

  digitalWrite(MUX_MQ2, d1);
  digitalWrite(MUX_LDR, d2);
  digitalWrite(MUX_LDR2, d3);
  digitalWrite(MUX_TNEM, d4);

  delay(10); //değişene kadar kısa bir bekleme
}

void LapsanaSensorler::dhtDurumYenidenDene() {
  for(int i = 0; i <= YENIDEN_DENEME; i++) {
    dhtDurum = dht.getStatusString() == "OK" ? TAMAM : HATA;
    if (dhtDurum == TAMAM) break;
    delay(1000);
  }
}

void LapsanaSensorler::dhtOlcumYenidenDene(float *olcum, bool sicaklik) {
  float yeniOlcum = NAN;
  
  for(int i = 0; i <= YENIDEN_DENEME; i++) {
    yeniOlcum = sicaklik ? dht.getTemperature() : dht.getHumidity();
    if (!isnan(yeniOlcum)) break;
    delay(1000);
  }

  *olcum = yeniOlcum;
}

float LapsanaSensorler::analogOrnekle() {
  float ornekler = 0.0;

  for(int i = 0; i <= ANALOG_ORNEK_SAYISI; i++) {
    ornekler += analogRead(MUX_PIN);
    delay(ANALOG_ORNEK_BEKLEME);
  }

  return ornekler;
}

#pragma endregion