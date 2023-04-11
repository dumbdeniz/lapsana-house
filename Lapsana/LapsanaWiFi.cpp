#include <Arduino.h> //Arduino kütüphanesi
#include <ESP8266WiFi.h> //Wifi kütüphanesi
#include <ESP8266HTTPClient.h> //HTTP client
#include <WiFiClientSecureBearSSL.h>

#include "LapsanaWiFi.h" //C++ header dosyası
#include "LapsanaSensorler.h"
#include "LapsanaCihazlar.h"
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

void LapsanaWiFi::httpsGonder(SensorDegerler &degerler, SensorDurumlar &durumlar, CihazDurumlar &cihazDurumlar) {
  Serial.println("\nHTTPS:");

  //Wi-Fi bağlantısını kontrol et
  if (!WiFi.isConnected()) {
    Serial.println("- Wi-Fi bağlantısı yok.");
    return;
  }

  //BearSSL::WiFiClientSecure *client = new BearSSL::WiFiClientSecure();
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  //client->setFingerprint(fingerprint_sni_cloudflaressl_com);
  client->setInsecure();

  HTTPClient https;

  //Bağlantı kurulabildi mi?
  //https://www.howsmyssl.com/a/check - önce SSL deneyelim
  if (!https.begin(*client, ISTEK_URL)) {
    Serial.println("- Bağlantı kurulamadı.");
    return;
  }

  //verileri JSON olarak formatla
  char jsonVeri[256];
  json(jsonVeri, degerler, durumlar);

  char sifreliVeri[320];
  sifrele(jsonVeri, sifreliVeri);

  //isteğe verileri ekle
  https.addHeader("veri", sifreliVeri);
  https.addHeader("api-key", API_KEY);

  //isteği gönder
  int code = https.GET();
  if (code < 0) {
    Serial.println("- İstek gönderilirken bir sorun oluştu.");
    return;
  }

  Serial.print("- HTTP Kodu: ");
  Serial.println(code);

  //yanit kodu geçerli mi
  if (code != HTTP_CODE_OK && code != HTTP_CODE_MOVED_PERMANENTLY) {
    Serial.println("- Beklenen yanıt alınamadı.");
    return;
  }

  //yaniti al
  char *yanit = (char*)https.getString().c_str();
  
  Serial.print("- HTTP Yanıt: ");
  Serial.println(yanit);

  char cozulmusVeri[16];
  sifreCoz(yanit, cozulmusVeri);

  https.end(); //artık işimiz yok

  //çözdüğümüz veri geçerli mi
  if (cozulmusVeri[0] != '1' && cozulmusVeri[0] != '0') {
    Serial.print("- Yanıt doğru biçimde çözülemedi: ");
    Serial.println(cozulmusVeri);
    return;
  }

  cihazDurumlar.suMotoru = cozulmusVeri[0] == '1' ? true : false;
  cihazDurumlar.vana = cozulmusVeri[1] == '1' ? true : false;
  cihazDurumlar.lamba = cozulmusVeri[2] == '1' ? true : false;
  cihazDurumlar.isitici = cozulmusVeri[3] == '1' ? true : false;
  cihazDurumlar.fan = cozulmusVeri[4] == '1' ? true : false;
  cihazDurumlar.pencere = cozulmusVeri[5] == '1' ? true : false;

  Serial.println("-------------------");
}

#pragma endregion