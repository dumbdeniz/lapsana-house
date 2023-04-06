#ifndef LAPSANAUTILS_H
#define LAPSANAUTILS_H

#include <Arduino.h>

void blink();

void seriYazdir(bool dhtDurumu, bool mq2Durumu, float sicaklik, float nem, float gaz, float lpg, float co, float duman, float isik, float toprakNem);

#endif