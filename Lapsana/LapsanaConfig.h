//Ağa bağlanmak için Wi-Fi adı ve şifresi
#define SSID "Sseml_Sera"
#define PASS "Password1!"

//Verilerin gönderileceği sunucu adresi
#define ISTEK_URL "https://192.168.16.88/e-seracik/node.php"

//Gönderilecek verileri formatlamak için kullanılacak JSON şablonu
#define JSON_FORMAT "{\"degerler\":{\"sicaklik\":%d,\"nem\":%d,\"gaz\":%d,\"isik\":%d,\"toprakNem\":%d,\"suSeviye\":%s},\"durum\":{\"sensorler\":{\"dht11\":%d,\"mq2\":%d,\"ldr\":%d,\"toprakNem\":%d},\"cihazlar\":{\"isitici\":%s,\"lamba\":%s,\"pompa\":%s,\"fan\":%s,\"pencere\":%s}}}"

//Sunucuya erişmek için kullanılacak API anahtarı
#define API_KEY "deniz3169"

//Verileri şifrelemek için kullanılan ortak anahtar
//AES-256 - 32 karakterlik
#define AES_KEY "B&E)H@McQfTjWnZqB&E)H@McQfTjWnZq"

//Verileri şifrelerken algoritmanın kullanacağı IV
//İdeal olarak rastgele olması gerekirdi, hatta gerekiyor
#define AES_IV "u8x/A?D(G+KbPeSh"

//Sıcaklık ve nem sensörü pini
#define DHT_PIN D5

//Multiplexer pinleri
#define MUX_PIN A0
#define MUX_MQ2 D1
#define MUX_LDR D2
#define MUX_LDR2 D3
#define MUX_TNEM D4

//Su seviyesi sensörü pini
#define SU_SEVIYE_PIN D7

//Sensör açıldıktan sonra doğru ölçüm alabilmesi için ısınma süresi. (dk)
//Önerilen süre 15-20, 30 dakika. Eğer yakın bir zamanda kullanılmışsa bu 5 dakikaya düşürülebilir.
#define MQ2_ISINMA_SURESI 5

//Isınma süresini atlamak için 3V'a bağlanacak pin.
//Isınma süresini 0 olarak ayarlayarakta bu işlem atlanabilir.
#define MQ2_ISINMA_ATLAMA D0 //! AYARLANMADI

//Alınan analog ölçümler için ne kadar ölçüm alınıp ortalanmalı ve ne kadar beklenmeli
#define ANALOG_ORNEK_SAYISI 3
#define ANALOG_ORNEK_BEKLEME 25

//Sensörden gelen analog veri bu değerin altında ise bir sorun olduğunu bildirir.
#define ANALOG_HATA_SINIRI 15

//Ölçüm ve veri gönderimi aralığı
#define SENSOR_ARALIK 10000

//Bir sorun olması durumda tekrar deneme sayısı
#define YENIDEN_DENEME 3

//Durum LED'i
#define DURUM_LED D0 //! AYARLANMADI
