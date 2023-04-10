#ifndef LAPSANAWIFI_H
#define LAPSANAWIFI_H

#include <Arduino.h>
#include "LapsanaSensorler.h"
#include "LapsanaCihazlar.h"

class LapsanaWiFi {
  public:
    void init();

    void denetle();

    void httpsGonder(SensorDegerler &degerler, SensorDurumlar &durumlar, CihazDurumlar &cihazDurumlar);
};

#endif