#ifndef LAPSANAUTILS_H
#define LAPSANAUTILS_H

#include <Arduino.h>

#include "LapsanaSensorler.h"
#include "LapsanaCihazlar.h"
#include "LapsanaConfig.h"

void blink();

void json(char* cikis, const SensorDegerler &degerler, const SensorDurumlar &durumlar);

void sifrele(char giris[], char* cikis);

void sifreCoz(char giris[], char* cikis);

void sensorlerYazdir(SensorDegerler degerler, SensorDurumlar durumlar);

void cihazlarYazdir(CihazDurumlar durumlar);

#endif