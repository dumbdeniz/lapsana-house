#ifndef LAPSANASENSORLER_H
#define LAPSANASENSORLER_H

#include <Arduino.h>

enum class Sensor {
  DHT11, MQ2, LDR, LDR2, TOPRAK_NEM, SU_SEVIYE
};

enum SensorDurum {
  TAMAM, HATA, OLCUYOR, ISINIYOR
};

class LapsanaSensorler {
  public:
    LapsanaSensorler();

    void init();

    float sicaklik();

  private:
    void mux(Sensor sensor);

    void dhtDurumYenidenDene();

    void dhtOlcumYenidenDene(float *olcum, bool sicaklik);
};

#endif