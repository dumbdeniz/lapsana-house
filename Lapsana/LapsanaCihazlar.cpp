#include <Arduino.h> //Arduino kütüphanesi
#include <Servo.h>
#include "LapsanaSensorler.h"
#include "LapsanaCihazlar.h"

#include "LapsanaCihazlar.h" //C++ header dosyası
#include "LapsanaConfig.h" //Ayarları içeren dosya

Servo _servo;

void LapsanaCihazlar::init() {
  //Cihaz pinlerini ayarla
  pinMode(ISITICI_PIN, OUTPUT);
  pinMode(LAMBA_PIN, OUTPUT);
  pinMode(SU_MOTORU_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  //Servoyu hazırla
  _servo.attach(SERVO_PIN, 500, 2400);
  _servo.write(SERVO_KAPALI);

  //gerekiyorsa digitalWrite ile hepsini kapat.
}

void LapsanaCihazlar::hallet(CihazDurumlar &durumlar) {
  digitalWrite(ISITICI_PIN, durumlar.isitici);
  digitalWrite(LAMBA_PIN, durumlar.lamba);
  digitalWrite(SU_MOTORU_PIN, durumlar.suMotoru);
  digitalWrite(FAN_PIN, durumlar.fan);
  
  _servo.write(durumlar.pencere ? SERVO_ACIK : SERVO_KAPALI);
}