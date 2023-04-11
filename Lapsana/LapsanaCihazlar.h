#ifndef LAPSANACIHAZLAR_H
#define LAPSANACIHAZLAR_H

#include <Arduino.h>
#include "LapsanaSensorler.h"
#include "LapsanaCihazlar.h"

enum class Cihaz {
  ISITICI, LAMBA, SU_MOTORU, FAN, PENCERE
};

struct CihazDurumlar {
  bool isitici;
  bool lamba;
  bool suMotoru;
  bool fan;
  bool pencere;
};

class LapsanaCihazlar {
  public:
    void init();

    void hallet(CihazDurumlar &durumlar);
};

#endif