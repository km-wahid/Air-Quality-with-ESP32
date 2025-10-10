#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

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
#define MQ135PIN 34  // ESP32 ADC pin connected to MQ-135 analog output

WiFiClient espClient;
PubSubClient client(espClient);

// -------------------- WIFI CONNECT --------------------
void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

// -------------------- MQTT CONNECT --------------------
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" Connected!");
    } else {
      Serial.print(" Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 2 seconds");
      delay(2000);
    }
  }
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);
  dht.begin();
  connectWiFi();
  client.setServer(mqttServer, mqttPort);
}

// -------------------- LOOP --------------------
void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Read DHT22 data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature)) temperature = 0;
  if (isnan(humidity)) humidity = 0;

  // Read MQ-135 analog value
  int mq135Raw = analogRead(MQ135PIN);
  float airQuality = mq135Raw; // raw analog value, 0-4095

  // Create JSON object
  StaticJsonDocument<256> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["air_quality"] = airQuality;
  doc["timestamp"] = millis();

  char buffer[256];
  serializeJson(doc, buffer);

  // Publish
  client.publish(mqttTopic, buffer);
  Serial.print("Published: ");
  Serial.println(buffer);

  delay(5000); // every 5s
}
