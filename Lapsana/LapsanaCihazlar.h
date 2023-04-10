#ifndef LAPSANACIHAZLAR_H
#define LAPSANACIHAZLAR_H

#include <Arduino.h>

enum class Cihaz {
  SU_MOTORU, VANA, LAMBA, ISITICI, FAN, PENCERE
};

enum CihazDurum {
  KAPALI, ACIK
};

struct CihazDurumlar {
  bool suMotoru;
  bool vana;
  bool lamba;
  bool isitici;
  bool fan;
  bool pencere;
};

class LapsanaCihazlar {
  public:
    void init();

    void hallet(CihazDurumlar durumlar);

  private:
    void dmux(Cihaz cihaz);
};

#endif