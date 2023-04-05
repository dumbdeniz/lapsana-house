#include <Arduino.h> //Arduino kütüphanesi
#include <DHT.h> //Sıcaklık ve nem sensörü kütüphanesi - indirmek için https://cdn.demirdelen.net/dht
#include <MQ2.h> //Gaz sensörü kütüphanesi - indirmek için https://cdn.demirdelen.net/mq2
#include "LapsanaSensorler.h" //C++ header dosyası
#include "LapsanaConfig.h" //Ayarları içeren dosya

DHT dht; //Sıcaklık ve nem sensörü tanımı
int sicaklik, nem;

MQ2 mq2(MUX_PIN); //Gaz sensörü tanımı
float raw, lpg, co, duman;

int isik, toprakNem;

//Sensör durumları, hata var mı, yok mu vb. kontrol etmek için;
SensorDurum dhtDurum = TAMAM, mq2Durum = ISINIYOR, ldrDurum = TAMAM, toprakNemDurum = TAMAM, suSeviyeDurum = TAMAM;

LapsanaSensorler::LapsanaSensorler() {
  //boş
}

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
  if (digitalRead(MQ2_ISINMA_ATLAMA)) {
    mq2Durum = TAMAM;
    mux(Sensor::MQ2); //sensöre geç
    mq2.begin();
  } else {
    mq2Durum = ISINIYOR;
  }
}

float LapsanaSensorler::sicaklik(bool yenidenDene = true) {
  if (dhtDurum != TAMAM && yenidenDene) {
    dhtDurumYenidenDene();
    if (dhtDurum != TAMAM) return NAN;
  }

  float sicaklik = dht.getTemperature();

  //ölçüm başarısızsa tekrar dene
  if (isnan(sicaklik) && yenidenDene) {
    dhtOlcumYenidenDene(&sicaklik, true);
    if (isnan(sicaklik)) return NAN;
  }

  return sicaklik;
}

float LapsanaSensorler::nem(bool yenidenDene = true) {
  if (dhtDurum != TAMAM && yenidenDene) {
    dhtDurumYenidenDene();
    if (dhtDurum != TAMAM) return NAN;
  }

  float nem = dht.getHumidity();

  //ölçüm başarısızsa tekrar dene
  if (isnan(nem) && yenidenDene) {
    dhtOlcumYenidenDene(&nem, false);
    if (isnan(nem)) return NAN;
  }

  return nem;
}

#pragma endregion

#pragma region Private Kodlar

void LapsanaSensorler::mux(Sensor sensor) {
  int d1 = 0, d2 = 0, d3 = 0, d4 = 0;

  switch (sensor) {
    case MQ2: d1 = 1; break;
    case LDR: d2 = 1; break;
    case LDR2: d3 = 1; break;
    case TOPRAK_NEM: d4 = 1; break;
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

#pragma endregion