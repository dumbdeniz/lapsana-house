#include <Arduino.h> //Arduino kütüphanesi
#include <ESP8266WiFi.h> //Wifi kütüphanesi
#include <ESP8266HTTPClient.h> //HTTP client
#include <WiFiClientSecureBearSSL.h>

#include "LapsanaWiFi.h" //C++ header dosyası
#include "LapsanaUtils.h"
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
    blink();
    delay(1000);
  }
}

void LapsanaWiFi::denetle() {  
  //ESP tekrar ağa bağlanana kadar sorun olduğunu bildir
  while(!WiFi.isConnected()) { 
    blink();
    delay(1000);
  }
}

int LapsanaWiFi::httpsGonder() {
  //Wi-Fi bağlantısını kontrol et
  if (!WiFi.isConnected()) {
    Serial.println("- Wi-Fi bağlantısı yok.");
    return -1;
  }

  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  //client->setFingerprint(fingerprint_sni_cloudflaressl_com);
  client->setInsecure();

  HTTPClient https;

  //Bağlantı kurulabildi mi?
  //https://www.howsmyssl.com/a/check - önce SSL deneyelim
  if (!https.begin(*client, ISTEK_URL)) {
    Serial.println("- Bağlantı kurulamadı.");
    return -1;
  }

  //verileri ekle
  //https.addHeader("Content-Type", "application/json");
  //https.addHeader("Content", "{}");
  //https.addHeader("api_key", API_KEY);

  //isteği gönder
  int code = https.GET();
  if (code < 0) {
    Serial.println("- İstek gönderilirken bir sorun oluştu.");
    return -1;
  }

  Serial.print("- HTTP Kodu: ");
  Serial.println(code);

  if (code != HTTP_CODE_OK && code != HTTP_CODE_MOVED_PERMANENTLY) {
    Serial.println("- Beklenen yanıt alınamadı.");
    return -1;
  }

  Serial.print("- HTTP Yanıt:");
  Serial.println(https.getString());

  return code;
}

#pragma endregion