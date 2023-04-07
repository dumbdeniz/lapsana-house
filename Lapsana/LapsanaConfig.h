//Ağa bağlanmak için Wi-Fi adı ve şifresi
#define SSID "Sseml_Sera"
#define PASS "Password1!"

//Sıcaklık ve nem sensörü pini
#define DHT_PIN D4

//Multiplexer pinleri
#define MUX_PIN A0
#define MUX_MQ2 D5
#define MUX_LDR D6
#define MUX_LDR2 D7
#define MUX_TNEM D8

//Su seviyesi sensörü pini
#define SU_SEVIYE_PIN D0

//Sensör açıldıktan sonra doğru ölçüm alabilmesi için ısınma süresi. (dk)
//Önerilen süre 15-20, 30 dakika. Eğer yakın bir zamanda kullanılmışsa bu 5 dakikaya düşürülebilir.
#define MQ2_ISINMA_SURESI 5

//Isınma süresini atlamak için 3V'a bağlanacak pin.
//Isınma süresini 0 olarak ayarlayarakta bu işlem atlanabilir.
#define MQ2_ISINMA_ATLAMA D0

//Alınan analog ölçümler için ne kadar ölçüm alınıp ortalanmalı ve ne kadar beklenmeli
#define ANALOG_ORNEK_SAYISI 5
#define ANALOG_ORNEK_BEKLEME 100

//Ölçüm ve veri gönderimi aralığı
#define SENSOR_INTERVAL 10000

//Bir sorun olması durumda tekrar deneme sayısı
#define YENIDEN_DENEME 3

//Durum LED'i
#define DURUM_LED D0

//Verilerin gönderileceği sunucu adresi
#define ISTEK_URL "http://192.168.16.88/e-seracik/node.php"

//Sunucuya erişmek için kullanılacak API anahtarı
#define API_KEY "deniz3169"