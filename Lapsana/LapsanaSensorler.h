#ifndef LAPSANASENSORLER_H
#define LAPSANASENSORLER_H

#include <Arduino.h>

enum class Sensor {
  DHT11, MQ2, LDR, LDR2, TOPRAK_NEM, SU_SEVIYE
};

enum SensorDurum {
  TAMAM = 1, ISINIYOR = 0, HATA = -1
};

struct SensorDegerler {
  float sicaklik;
  float nem;
  float gaz;
  float isik;
  float toprakNem;
  bool suSeviyesi;
};

struct SensorDurumlar {
  int dht11;
  int mq2;
  int ldr;
  int toprakNem;
};

class LapsanaSensorler {
  public:
    void init();

    void hallet(SensorDegerler &degerler, SensorDurumlar &durumlar);

  private:
    void sicaklik(bool);

    void nem(bool);

    void gaz();

    void isik();

    void toprakNem();

    bool suSeviyesi();

    void mux(Sensor sensor);

    void mq2Denetle();

    void dhtDurumYenidenDene();

    void dhtOlcumYenidenDene(bool sicaklik);

    float analogOrnekle();
};

#endif