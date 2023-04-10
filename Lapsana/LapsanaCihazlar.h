#ifndef LAPSANACIHAZLAR_H
#define LAPSANACIHAZLAR_H

#include <Arduino.h>

enum class Cihaz {
  SU_MOTORU, VANA, LAMBA, ISITICI, FAN, SERVO
};

enum CihazDurum {
  KAPALI, ACIK
};

struct CihazDurumlar {
  int suMotoru;
  int vana;
  int lamba;
  int isitici;
  int fan;
  int servo;
};

class LapsanaCihazlar {
  public:
  private:
}

#endif