import json
import paho.mqtt.client as mqtt
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

# -------------------- MQTT CONFIG --------------------
BROKER_HOST = "localhost"      # Your host IP or Docker exposed port
BROKER_PORT = 1883
TOPIC = "esp32/sensors/data"

# -------------------- InfluxDB CONFIG --------------------
INFLUX_URL = "http://localhost:8086"   # Docker InfluxDB exposed on host
INFLUX_TOKEN = "mysecrettoken123"      # Token from docker-compose setup
INFLUX_ORG = "airquality_org"
INFLUX_BUCKET = "sensor_data"

# -------------------- INIT INFLUXDB CLIENT --------------------
try:
    influx_client = InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)
    write_api = influx_client.write_api(write_options=SYNCHRONOUS)
    print("✅ Connected to InfluxDB successfully.")
except Exception as e:
    print("❌ Failed to connect to InfluxDB:", e)

# -------------------- MQTT CALLBACK --------------------
def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        temperature = float(payload.get("temperature", 0))
        humidity = float(payload.get("humidity", 0))
        air_quality = float(payload.get("air_quality", 0))  # MQ-135 sensor value

        # Construct InfluxDB data point
        point = (
            Point("sensor_data")
            .tag("device", "esp32")
            .field("temperature", temperature)
            .field("humidity", humidity)
            .field("air_quality", air_quality)
        )

        # Write the data point to InfluxDB
        write_api.write(bucket=INFLUX_BUCKET, org=INFLUX_ORG, record=point)
        print(f"✅ Saved to InfluxDB: {payload}")

    except Exception as e:
        print("❌ Error writing to InfluxDB:", e)

# -------------------- MQTT CONNECT --------------------
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Connected to MQTT Broker!")
        client.subscribe(TOPIC)
        print(f"📡 Subscribed to topic: {TOPIC}")
    else:
        print(f"❌ Failed to connect, return code {rc}")

def start_mqtt_listener():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    try:
        client.connect(BROKER_HOST, BROKER_PORT, 60)
        print(f"🚀 Connecting to MQTT Broker at {BROKER_HOST}:{BROKER_PORT}")
        client.loop_forever()
    except Exception as e:
        print("❌ MQTT connection error:", e)

# -------------------- MAIN --------------------
if __name__ == "__main__":
    start_mqtt_listener()
