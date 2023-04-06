#include <Arduino.h> //Arduino kütüphanesi
#include <ESP8266WiFi.h> //Wifi kütüphanesi

#include "LapsanaWiFi.h" //C++ header dosyası
#include "LapsanaConfig.h" //Ayarları içeren dosya

#pragma region Public Kodlar

void LapsanaWiFi::init() {
  WiFi.mode(WIFI_STA); //Modu istasyon olarak ayarla
  WiFi.begin(SSID, PASS); //Wi-Fi ağına bağlan

  Serial.print("SSID : ");
  Serial.println(SSID);
  
  //Wi-Fi ağına bağlanana kadar veya maksimum 15 saniye bekleyene kadar
  for (int i = 0; i <= 15 && WiFi.status() != WL_CONNECTED; i++) {
    delay(1000);
    Serial.print(".");    
  }
  
  if (WiFi.isConnected()) {
    Serial.println("Bağlandı");

    Serial.print("IP : ");
    Serial.println(WiFi.localIP());
    return;
  }

  Serial.println("Başarısız");

  //Reset atılıncıya kadar sorun olduğunu bildir
  while(true) {
    Serial.println("blink()");
    delay(1000);
  }
}

void LapsanaWiFi::denetle() {  
  //ESP tekrar ağa bağlanana kadar sorun olduğunu bildir
  while(!WiFi.isConnected()) { 
    Serial.println("blink()");
    delay(1000);
  }
}

#pragma endregion