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

  digitalWrite(ISITICI_PIN, HIGH);
  digitalWrite(LAMBA_PIN, HIGH);
  digitalWrite(SU_MOTORU_PIN, HIGH);

  analogWrite(FAN_PIN, 0);
}

void LapsanaCihazlar::hallet(CihazDurumlar &durumlar) {
  digitalWrite(ISITICI_PIN, !durumlar.isitici);
  digitalWrite(LAMBA_PIN, !durumlar.lamba);
  digitalWrite(SU_MOTORU_PIN, !durumlar.suMotoru);

  analogWrite(FAN_PIN, durumlar.fan ? FAN_HIZ : 0);

  //yavaşça pencereyi aç
  for (int i = SERVO_KAPALI; i > SERVO_ACIK && durumlar.pencere; i -= SERVO_ADIM) {
    _servo.write(i);
    delay(SERVO_ARALIK);
  }

  //yavaşça pencereyi kapat
  for (int i = SERVO_ACIK; i < SERVO_KAPALI && !durumlar.pencere; i += SERVO_ADIM) {
    _servo.write(i);
    delay(SERVO_ARALIK);
  }

  //_servo.write(durumlar.pencere ? SERVO_ACIK : SERVO_KAPALI);
}