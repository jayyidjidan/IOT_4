#define BLYNK_TEMPLATE_ID "TMPL6McmqeLhb"
#define BLYNK_TEMPLATE_NAME "Monitoring"
#define BLYNK_AUTH_TOKEN "SuOvCZMxk9COdttTwT0UH1PuWWAqiHyQ"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h>  // Ganti dari BlynkTimer ke SimpleTimer

// --- Konfigurasi WiFi ---
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// --- Konfigurasi pin ---
#define DHTPIN 14
#define DHTTYPE DHT22
#define LDR_PIN 34

// --- Inisialisasi objek ---
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
SimpleTimer timer; // Gunakan SimpleTimer

void sendSensor() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  int cahaya = analogRead(LDR_PIN);

  // Kirim ke Serial Monitor
  Serial.print("Suhu: "); Serial.print(suhu);
  Serial.print(" C | Kelembapan: "); Serial.print(kelembapan);
  Serial.print(" % | Cahaya: "); Serial.println(cahaya);

  // Kirim ke Blynk
  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, kelembapan);
  Blynk.virtualWrite(V2, cahaya);

  // Tampilkan ke LCD
  lcd.setCursor(0, 0);
  lcd.print("Suhu     : ");
  lcd.print(suhu, 1);
  lcd.print((char)223);
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  lcd.print("Kelembapan: ");
  lcd.print(kelembapan, 1);
  lcd.print("%   ");

  lcd.setCursor(0, 2);
  lcd.print("Cahaya   : ");
  lcd.print(cahaya);
  lcd.print("     ");

  lcd.setCursor(0, 3);
  lcd.print("Monitoring Aktif...   ");
}

void setup() {
  Serial.begin(115200);

  // Inisialisasi sensor dan LCD
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   Sistem Monitoring");
  delay(2000);
  lcd.clear();

  // Hubungkan ke WiFi & Blynk
  WiFi.begin(ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Jalankan pengiriman data sensor tiap 2 detik
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();  // Jalankan SimpleTimer
}
