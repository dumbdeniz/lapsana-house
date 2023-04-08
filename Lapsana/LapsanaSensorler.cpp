#include <Arduino.h> //Arduino kütüphanesi
#include <DHT.h> //Sıcaklık ve nem sensörü kütüphanesi - indirmek için https://cdn.demirdelen.net/_dht
#include <MQ2.h> //Gaz sensörü kütüphanesi - indirmek için https://cdn.demirdelen.net/_mq2

#include "LapsanaSensorler.h" //C++ header dosyası
#include "LapsanaConfig.h" //Ayarları içeren dosya

DHT _dht; //Sıcaklık ve nem sensörü tanımı
MQ2 _mq2(MUX_PIN); //Gaz sensörü tanımı

//Sensörlerden ölçülen değerler
float _sicaklik, _nem, _gaz, _isik, _toprakNem;

//Sensör durumları, hata var mı, yok mu vb. kontrol etmek için;
SensorDurum _dhtDurum = TAMAM, _mq2Durum = ISINIYOR, _ldrDurum = TAMAM, _toprakNemDurum = TAMAM;

#pragma region Public Kodlar

void LapsanaSensorler::init() {
  //Multiplexer pinlerini ayarla
  pinMode(MUX_MQ2, OUTPUT);
  pinMode(MUX_LDR, OUTPUT);
  pinMode(MUX_LDR2, OUTPUT);
  pinMode(MUX_TNEM, OUTPUT);

  //DHT11'i hazırla
  _dht.setup(DHT_PIN, DHT::DHT11);

  //DHT sensör durumunu kontrol et
  dhtDurumYenidenDene();

  //Gaz sensörünün ısınma süresini atlama pini bağlanmışsa geç ve sensörü hazırla, değilse ısınıyor olarak ayarla
  //Isınma süresi içinde herhangi bir ölçüm isteği gelirse sensör ısınana kadar veri göndermez
  if (digitalRead(MQ2_ISINMA_ATLAMA) || MQ2_ISINMA_SURESI == 0) {
    _mq2Durum = TAMAM;
    mux(Sensor::MQ2); //sensöre geç
    _mq2.begin();
  } else {
    _mq2Durum = ISINIYOR;
  }
}

void LapsanaSensorler::hallet(SensorDegerler &degerler, SensorDurumlar &durumlar) {
  SensorDegerler yeniDegerler;
  SensorDurumlar yeniDurumlar;

  //Sıcaklık ve nemi ölç
  //Yeniden denemeyi kapatmak için parametre olarak false verilmeli
  sicaklik(false);
  nem(false);

  //MQ2 ısınma durumunu kontrol et, eğer ısınmışsa ölçümleri al
  mq2Denetle();
  if (_mq2Durum == TAMAM) gaz();

  //Geriye kalan sensörlerden ölçümleri al
  isik();
  toprakNem();

  //Durumları ayarla
  yeniDurumlar.dht11 = _dhtDurum;
  yeniDurumlar.mq2 = _mq2Durum;
  yeniDurumlar.ldr = _ldrDurum;
  yeniDurumlar.toprakNem = _toprakNemDurum;

  //Değerleri ayarla
  //Eğer sensör hatalıysa değer -1 olacak
  yeniDegerler.sicaklik = _dhtDurum == TAMAM ? _sicaklik : -1;
  yeniDegerler.nem = _dhtDurum == TAMAM ? _nem : -1;
  yeniDegerler.gaz = _mq2Durum == TAMAM ? _gaz : -1;
  yeniDegerler.isik = _ldrDurum == TAMAM ? _isik : -1;
  yeniDegerler.toprakNem = _toprakNemDurum == TAMAM ? _toprakNem : -1;
  yeniDegerler.suSeviyesi = suSeviyesi(); //sadece true yada false döndürdüğü için hata bulma yapılmıyor

  //Değerleri ve durumu referansı verilen parametreye yaz
  degerler = yeniDegerler;
  durumlar = yeniDurumlar;
}

#pragma endregion

#pragma region Private Kodlar

void LapsanaSensorler::sicaklik(bool yenidenDene) {
  if (_dhtDurum != TAMAM && yenidenDene) {
    dhtDurumYenidenDene();
    if (_dhtDurum != TAMAM) return;
  }

  _sicaklik = _dht.getTemperature();

  //ölçüm başarısızsa tekrar dene
  if (isnan(_sicaklik)) {
    if (yenidenDene) {
      dhtOlcumYenidenDene(true);
      if (isnan(_sicaklik)) _dhtDurum = HATA;
      return;
    }
    _dhtDurum = HATA;
  }
}

void LapsanaSensorler::nem(bool yenidenDene) {
  if (_dhtDurum != TAMAM && yenidenDene) {
    dhtDurumYenidenDene();
    if (_dhtDurum != TAMAM) return;
  }

  _nem = _dht.getHumidity();

  //ölçüm başarısızsa tekrar dene
  if (isnan(_nem)) {
    if (yenidenDene) {
      dhtOlcumYenidenDene(false);
      if (isnan(_nem)) _dhtDurum = HATA;
      return;
    }
    _dhtDurum = HATA;
  }
}

void LapsanaSensorler::gaz() {
  if (_mq2Durum != TAMAM) return;

  mux(Sensor::MQ2);

  _gaz = analogOrnekle() / ANALOG_ORNEK_SAYISI;

  if (_gaz <= ANALOG_HATA_SINIRI) _mq2Durum = HATA;
}

void LapsanaSensorler::isik() {
  float olcumler = 0.0;

  mux(Sensor::LDR); //ilk LDR sensörüne geç

  olcumler += analogOrnekle();

  mux(Sensor::LDR2); //ikinci LDR sensörüne geç

  olcumler += analogOrnekle();

  _isik = olcumler / (ANALOG_ORNEK_SAYISI * 2);

  if (_isik <= ANALOG_HATA_SINIRI) _ldrDurum = HATA;
}

void LapsanaSensorler::toprakNem() {
  mux(Sensor::TOPRAK_NEM);

  _toprakNem = analogOrnekle() / ANALOG_ORNEK_SAYISI;

  if (_toprakNem <= ANALOG_HATA_SINIRI) _toprakNemDurum = HATA;
}

bool LapsanaSensorler::suSeviyesi() {
  return digitalRead(SU_SEVIYE_PIN);
}

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

void LapsanaSensorler::mq2Denetle() {
  if (_mq2Durum == ISINIYOR) {
    unsigned long isinmaSuresi = MQ2_ISINMA_SURESI * 60 * 1000; //dakika * saniye * milisaniye

    //Isınma süresi geçtiyse veya atlandıysa kalibrasyona başla ve sensörü hazırla
    if (digitalRead(MQ2_ISINMA_ATLAMA) || millis() >= isinmaSuresi) {
      mux(Sensor::MQ2);
      _mq2.begin();
      _mq2Durum = TAMAM;
    }
  }
}

void LapsanaSensorler::dhtDurumYenidenDene() {
  for(int i = 0; i <= YENIDEN_DENEME; i++) {
    _dhtDurum = _dht.getStatusString() == "OK" ? TAMAM : HATA;
    if (_dhtDurum == TAMAM) break;
    delay(1000);
  }
}

void LapsanaSensorler::dhtOlcumYenidenDene(bool sicaklik) {
  float yeniOlcum = NAN;
  
  for(int i = 0; i <= YENIDEN_DENEME; i++) {
    yeniOlcum = sicaklik ? _dht.getTemperature() : _dht.getHumidity();
    if (!isnan(yeniOlcum)) break;
    delay(1000);
  }

  sicaklik ? _sicaklik = yeniOlcum : _nem = yeniOlcum;
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