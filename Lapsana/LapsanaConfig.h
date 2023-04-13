//Ağa bağlanmak için Wi-Fi adı ve şifresi
#define SSID "Sseml_Sera"
#define PASS "Password1!"

//Verilerin gönderileceği sunucu adresi
#define ISTEK_URL "https://192.168.16.102/e-seracik/test.php"

//HTTPS bağlantısının kurulması en fazla ne kadar sürmeli?
#define HTTPS_TIMEOUT 2000

//Sitenin sertifikası
#define HTTPS_CERT ""

//Gönderilecek verileri formatlamak için kullanılacak JSON şablonu
#define JSON_FORMAT "{\"degerler\":{\"sicaklik\":%d,\"nem\":%d,\"gaz\":%d,\"isik\":%d,\"toprakNem\":%d,\"suSeviye\":%s},\"durum\":{\"sensorler\":{\"dht11\":%d,\"mq2\":%d,\"ldr\":%d,\"toprakNem\":%d},\"cihazlar\":{\"isitici\":%s,\"lamba\":%s,\"pompa\":%s,\"fan\":%s,\"pencere\":%s}}}"

//Sunucuya erişmek için kullanılacak API anahtarı
#define API_KEY "bb0c8cfb95dece85a728acd33e4135a5"

//Verileri şifrelemek için kullanılan ortak anahtar
//AES-256 - 32 karakterlik
#define AES_KEY "B&E)H@McQfTjWnZqB&E)H@McQfTjWnZq"

//Multiplexer pinleri
#define MUX_PIN A0
#define MUX_S0 D0
#define MUX_S1 D1

//Sıcaklık ve nem sensörü pini
#define DHT_PIN D2

//Cihaz pinleri
#define ISITICI_PIN D7
#define LAMBA_PIN D6
#define SU_MOTORU_PIN D5
#define FAN_PIN D3
#define SERVO_PIN 2 //D4

//Fan çalışma hızı
#define FAN_HIZ 155

//Servo çalışma aralığı (pencereye göre kalibre edilmeli)
#define SERVO_KAPALI 120
#define SERVO_ACIK 0

//Servo hız kontrolü
#define SERVO_ADIM 5
#define SERVO_ARALIK 25

//Sensör açıldıktan sonra doğru ölçüm alabilmesi için ısınma süresi. (dk)
//Önerilen süre 15-20, 30 dakika. Eğer yakın bir zamanda kullanılmışsa bu 5 dakikaya düşürülebilir.
#define MQ2_ISINMA_SURESI 5

//Alınan analog ölçümler için ne kadar ölçüm alınıp ortalanmalı ve ne kadar beklenmeli
#define ANALOG_ORNEK_SAYISI 5
#define ANALOG_ORNEK_BEKLEME 15

//Sensörden gelen analog veri bu değerin altında ise bir sorun olduğunu bildirir.
#define ANALOG_HATA_SINIRI 20

//Ölçüm ve veri gönderimi aralığı
#define SENSOR_ARALIK 10000

//Bir sorun olması durumda tekrar deneme sayısı
#define YENIDEN_DENEME 3