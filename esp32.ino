#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// -------------------- WIFI CONFIG --------------------
const char* ssid = "hideass";
const char* password = "Lifehacker2022";

// -------------------- MQTT CONFIG --------------------
const char* mqttServer = "172.25.4.23";  // your local machine IP
const int mqttPort = 1883;
const char* mqttTopic = "esp32/sensors/data";

// -------------------- DHT CONFIG --------------------
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// -------------------- MQ-135 CONFIG --------------------
#define MQ135PIN 34  // analog input pin

// -------------------- LED CONFIG --------------------
#define LED_PIN 2  // onboard LED

// -------------------- NTP CONFIG --------------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // UTC, update every 60s

WiFiClient espClient;
PubSubClient client(espClient);

// -------------------- ERROR HANDLER --------------------
void errorBlink() {
  Serial.println("⚠️ Sensor or connection error detected — blinking for 20s...");
  unsigned long start = millis();
  while (millis() - start < 20000) {  // blink for 20 seconds
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(200);
  }
  digitalWrite(LED_PIN, LOW);
  delay(2000); // rest 2 seconds before retry
}

// -------------------- WIFI CONNECT --------------------
void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_PIN, HIGH);
  Serial.println(" Connected!");
  timeClient.begin();
  timeClient.update();
}

// -------------------- MQTT CONNECT --------------------
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" Connected!");
      digitalWrite(LED_PIN, HIGH);
    } else {
      Serial.print(" Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying...");
      errorBlink(); // blink 20s on connection fail
    }
  }
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  connectWiFi();
  client.setServer(mqttServer, mqttPort);
}

// -------------------- LOOP --------------------
void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  timeClient.update();
  String timestamp = timeClient.getFormattedTime();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int mq135Raw = analogRead(MQ135PIN);
  float airQuality = mq135Raw;

  // Validate readings
  if (isnan(temperature) || isnan(humidity)) {
    errorBlink();
    return;
  }

  // Create JSON object
  StaticJsonDocument<256> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["air_quality"] = airQuality;
  doc["timestamp"] = timestamp;

  char buffer[256];
  serializeJson(doc, buffer);

  // Publish to MQTT
  if (client.publish(mqttTopic, buffer)) {
    Serial.print("✅ Published: ");
    Serial.println(buffer);

    // Blink once on successful publish
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("❌ MQTT publish failed!");
    errorBlink();
  }

  delay(10000); // every 10 seconds
}
