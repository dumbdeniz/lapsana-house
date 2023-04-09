#ifndef LAPSANAUTILS_H
#define LAPSANAUTILS_H

#include <Arduino.h>
#include "LapsanaSensorler.h"
#include "LapsanaConfig.h"

void blink();

void json(char* cikis, const SensorDegerler &degerler, const SensorDurumlar &durumlar);

void sifrele(char giris[], char* cikis);

void sifreCoz(char giris[], char* cikis);

void seriYazdir(SensorDegerler degerler, SensorDurumlar durumlar);

#endif