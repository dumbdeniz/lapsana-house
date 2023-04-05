#ifndef LAPSANASENSORLER_H
#define LAPSANASENSORLER_H

#include <Arduino.h>

enum class Sensor {
  DHT11, MQ2, LDR, LDR2, TOPRAK_NEM, SU_SEVIYE
};

enum SensorDurum {
  TAMAM, HATA, ISINIYOR
};

class LapsanaSensorler {
  public:
    LapsanaSensorler();

    void init();

    void mq2Denetle();

    float sicaklik(bool);

    float nem(bool);

    float gaz();

    float lpg();

    float co();

    float duman();

    float isik();

    float toprakNem();

    bool suSeviyesi();
    
    SensorDurum durum(Sensor sensor);

  private:
    void mux(Sensor sensor);

    void dhtDurumYenidenDene();

    void dhtOlcumYenidenDene(float *olcum, bool sicaklik);

    float analogOrnekle();
};

#endif