#include <Wire.h>
#include <DHT.h>
#include <FirebaseESP32.h>
#include <WiFi.h>

// Isikan sesuai pada Firebase
#define FIREBASE_HOST "https://monitoring-ruangan-fe509-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "F3NQP9GgTqzA385J7nMJzqEcF6voH25VWKqoixQN"

#define WIFI_SSID "Kandang Sapi Betina"
#define WIFI_PASSWORD "antipdi21"
// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;


DHT dht(5, DHT11);  //Pin, Jenis DHT

void setup() {

  Serial.begin(115200);
  dht.begin();
  pinMode(LED_BUILTIN, OUTPUT);

  // Koneksi ke Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  digitalWrite(LED_BUILTIN, 1);

}

void loop() {

  float k = dht.readHumidity();
  float s = dht.readTemperature();
  Serial.print(s);
  Serial.print(" || ");
  Serial.println(k);
  // Memberikan status suhu,kelembaban dan NH3 kepada firebase
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/suhu", s))
  {
    Serial.println("Suhu terkirim");
  } else
  {
    Serial.println("Suhu tidak terkirim");
    Serial.println("Karena: " + firebaseData.errorReason());
  }
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/kelembapan", k))
  {
    Serial.println("Kelembaban terkirim");
    Serial.println();
  } else
  {
    Serial.println("Kelembaban tidak terkirim");
    Serial.println("Karena: " + firebaseData.errorReason());
  }
  String FBStatus;
  if (Firebase.getString(firebaseData, "/Hasil_Pembacaan/Lampu", &FBStatus)) {
    if  (firebaseData.dataType() == "string"){
      if (FBStatus == "1") {
        digitalWrite(LED_BUILTIN, 1);
      }
      else if (FBStatus == "0") {
        digitalWrite(LED_BUILTIN, 0);
      }
    }
    }
    

  }