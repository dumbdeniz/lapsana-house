//Ağa bağlanmak için Wi-Fi adı ve şifresi
#define SSID "Sseml_Sera"
#define PASS "Password1!"

//Sıcaklık ve nem sensörü pini
#define DHT_PIN D5

//Multiplexer pinleri
//Toprak Nem    : (C0) : S0 ve S1 LOW
//Işık Seviyesi : (C1) : S0 HIGH, S1 LOW
//Gaz Sensörü   : (C2) : S0 LOW, S1 HIGH
#define MUX_PIN A0
#define MUX_S0 D6
#define MUX_S1 D7

//Ölçüm ve veri gönderimi aralığı
#define SENSOR_INTERVAL 10000

//Durum LED'i
#define STATUS_LED D8

//Verilerin gönderileceği sunucu adresi
#define ISTEK_URL "http://192.168.16.88/e-seracik/node.php"