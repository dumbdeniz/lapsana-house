#ifndef LAPSANAWIFI_H
#define LAPSANAWIFI_H

#include <Arduino.h>

class LapsanaWiFi {
  public:
    void init();

    void denetle();

    int httpsGonder();
};

#endif