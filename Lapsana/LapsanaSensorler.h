#ifndef LAPSANASENSORLER_H
#define LAPSANASENSORLER_H

#include <Arduino.h>
#include <DHT.h>
#include <MQ2.h>

class LapsanaSensorler {
  private:
    DHT dht;
    MQ2 mq2(MUX_PIN);
    
  public:
    LapsanaSensorler();

    void init();
};

#endif