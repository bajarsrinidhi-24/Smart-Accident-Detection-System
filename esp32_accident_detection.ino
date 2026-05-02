/* ================= BLYNK ================= */
#define BLYNK_TEMPLATE_ID "TMPL3AtD-7ZVb"
#define BLYNK_TEMPLATE_NAME "WIFI Control Car"
#define BLYNK_AUTH_TOKEN "asXJoBcLgzjud9VHTk_ec4lvTQASlIpn"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <math.h>

/* ================= WIFI ================= */
char ssid[] = "xxxx";
char pass[] = "xxxxx";

/* ================= THINGSPEAK ================= */
const char* host = "api.thingspeak.com";
#include "secrets.h"   // WRITE API KEY
WiFiClient client;

/* ================= SENSOR PINS ================= */
#define X_PIN 34
#define Y_PIN 35
#define Z_PIN 32

#define ZERO_G_X 2048
#define ZERO_G_Y 2048
#define ZERO_G_Z 2048
#define SENSITIVITY 410.0

#define MAG_THRESHOLD 1.5
#define TILT_THRESHOLD 45.0

unsigned long lastSendTime = 0;

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  Serial.println("SYSTEM READY");
}

/* ================= SENSOR FUNCTION ================= */
float readAxis(int pin, int zero_g) {
  int raw = analogRead(pin);
  return (raw - zero_g) / SENSITIVITY;
}

/* ================= LOOP ================= */
void loop() {

  float ax = readAxis(X_PIN, ZERO_G_X);
  float ay = readAxis(Y_PIN, ZERO_G_Y);
  float az = readAxis(Z_PIN, ZERO_G_Z);

  float magnitude = sqrt(ax*ax + ay*ay + az*az);
  float tilt = acos(abs(az) / magnitude) * 180.0 / PI;
  if (isnan(tilt)) tilt = 0;

  int accident = (magnitude > MAG_THRESHOLD || tilt > TILT_THRESHOLD) ? 1 : 0;

  Serial.println("\n------------------------");
  Serial.printf("AccX: %.2f | AccY: %.2f | AccZ: %.2f\n", ax, ay, az);
  Serial.printf("Magnitude: %.2f | Tilt: %.2f\n", magnitude, tilt);

  if (accident) Serial.println("ACCIDENT DETECTED");
  else Serial.println("NORMAL");

  if (millis() - lastSendTime > 15000) {

    Serial.println("\nConnecting to ThingSpeak...");

    if (client.connect(host, 80)) {

      String url = "/update?api_key=" + apiKey +
                   "&field1=" + String(ax, 3) +
                   "&field2=" + String(ay, 3) +
                   "&field3=" + String(az, 3) +
                   "&field4=" + String(accident);

      Serial.println("Sending Data:");
      Serial.println(url);

      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");

      long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println("Timeout waiting for response");
          client.stop();
          return;
        }
      }

      bool success = false;

      while (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);

        if (line.indexOf("200 OK") != -1) {
          success = true;
        }
      }

      if (success) {
        Serial.println("ThingSpeak UPDATE SUCCESS");
      } else {
        Serial.println("ThingSpeak UPDATE FAILED");
      }

    } else {
      Serial.println("Connection to ThingSpeak failed");
    }

    client.stop();
    lastSendTime = millis();
  }

  delay(1000);
}
