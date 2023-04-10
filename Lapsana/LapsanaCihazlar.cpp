#include <Arduino.h> //Arduino kütüphanesi

#include "LapsanaCihazlar.h" //C++ header dosyası
#include "LapsanaConfig.h" //Ayarları içeren dosya

#pragma region Public kodlar

void LapsanaCihazlar::init() {
  //cihazları hazırla
}

void LapsanaCihazlar::hallet(CihazDurumlar durumlar) {
  //duruma göre cihazları aç veya kapat
}

#pragma endregion

#pragma region Private kodlar

void LapsanaCihazlar::dmux(Cihaz cihaz) {
  int a = 0, b = 0, c = 0;

  //TODO: Hangi sensörler nasıl bağlanacak bul
}

#pragma endregion