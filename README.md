# 🌐 IoT-Based Air Quality Monitoring System

This project is a real-time IoT Air Quality Monitoring System that collects sensor data from an ESP32 and visualizes it through Grafana using InfluxDB as the time-series database. It provides live dashboards and historical analysis for environmental data such as temperature, humidity, and air quality index.

> **🟢 Note:** The entire system (InfluxDB + Grafana + Django backend) is deployed on AWS EC2, allowing remote access and scalability for multiple IoT devices.

---

## 🚀 System Overview

The system enables continuous monitoring of air quality through an IoT device (ESP32) equipped with sensors. Data is sent over Wi-Fi to a cloud-hosted InfluxDB instance, which stores all readings for further visualization in Grafana dashboards. The backend is hosted on AWS, ensuring high availability and secure connectivity between devices and the dashboard.

---

## 🧠 How the System Works

### 1. Data Collection (ESP32)

The ESP32 board reads sensor data from modules like:

- **DHT11/DHT22** → Temperature and Humidity
- **MQ135** → Air Quality / Gas concentration

The sensor readings are converted into structured JSON data. The ESP32 connects to Wi-Fi and sends this data to the AWS-hosted InfluxDB endpoint.

### 2. Data Transmission

Data is sent using the **HTTP POST** or **MQTT protocol** to the AWS server's InfluxDB endpoint.

**Example data payload:**

```json
{
  "temperature": 27.4,
  "humidity": 56.3,
  "air_quality": 135
}
```

### 3. Data Storage (InfluxDB on AWS)

The incoming data is written to **InfluxDB**, a time-series database running on an AWS EC2 instance. Each sensor reading is stored with a timestamp, enabling time-based querying and trend analysis.

### 4. Data Visualization (Grafana on AWS)

Grafana, hosted on the same AWS EC2 server, connects to InfluxDB as a data source. Dashboards are created to visualize:

- Real-time temperature, humidity, and air quality levels
- Historical trends and variations
- Threshold-based color-coded gauges (e.g., green, yellow, red)

---

## 🏗️ System Architecture

```
                ┌────────────────────────────┐
                │        Sensor Node         │
                │  (ESP32 + MQ135 + DHT22)  │
                └────────────┬──────────────┘
                             │ Wi-Fi
                             ▼
                ┌────────────────────────────┐
                │     Data Transmission      │
                │ (HTTP / MQTT over Wi-Fi)   │
                └────────────┬──────────────┘
                             │
                             ▼
        ┌──────────────────────────────────────────┐
        │             AWS EC2 Instance              │
        │  ┌────────────────────────────────────┐  │
        │  │          InfluxDB Server           │  │
        │  │ (Time-Series Database for Storage) │  │
        │  └────────────────────────────────────┘  │
        │  ┌────────────────────────────────────┐  │
        │  │           Grafana Dashboard         │  │
        │  │   (Data Visualization Interface)   │  │
        │  └────────────────────────────────────┘  │
        └──────────────────────────────────────────┘
```

☁️ **Cloud Deployment:** The entire backend stack (InfluxDB + Grafana + Django API) runs on an AWS EC2 instance, allowing ESP32 devices to push data remotely and Grafana dashboards to be accessed via a public IP.

---

## 📊 Dashboard Example

A Grafana dashboard can display:

- **Live gauges** for Temperature, Humidity, Air Quality
- **Historical graphs** showing hourly or daily variation
- **Color-coded alert zones** (e.g., Green < 20°C, Yellow < 30°C, Red > 30°C)
- **Real-time data refresh** using the InfluxDB time-series connection

---

## 🔄 Data Flow Visualization

```
┌───────────────────┐   ┌─────────────────────┐   ┌───────────────────┐   ┌───────────────────┐
│ ESP32 Sensor Node │──▶│ Wi-Fi Local Network │──▶│  InfluxDB (AWS)   │──▶│   Grafana (AWS)   │
│ Reads & formats   │   │  Transmits Data     │   │ Stores time-series│   │ Visualizes data   │
│   sensor data     │   │                     │   │       data        │   │   on dashboard    │
└───────────────────┘   └─────────────────────┘   └───────────────────┘   └───────────────────┘
```

**Step-by-Step Process:**

1. **Step 1:** ESP32 reads & formats sensor data
2. **Step 2:** Data sent over Wi-Fi
3. **Step 3:** InfluxDB stores the time-series data
4. **Step 4:** Grafana reads data and visualizes it on dashboards

---

## 🧩 Key Features

- 🌍 **AWS-Hosted Cloud Deployment**
- ⚡ **Real-time monitoring** of environmental data
- 📈 **Time-based trend visualization**
- 🔒 **Secure and scalable architecture**
- 🧱 **Modular system** — easily extendable with more sensors
- 🧠 **Open-source and IoT-friendly design**

---

## 🛠️ Technology Stack

| Component | Technology |
|-----------|------------|
| **IoT Device** | ESP32 |
| **Sensors** | DHT11/DHT22 (Temperature & Humidity), MQ135 (Air Quality) |
| **Communication** | HTTP / MQTT over Wi-Fi |
| **Database** | InfluxDB (Time-Series) |
| **Visualization** | Grafana |
| **Backend** | Django (Python) |
| **Cloud Platform** | AWS EC2 |

---

## 📦 Installation & Setup

### Prerequisites

- ESP32 development board
- DHT11/DHT22 and MQ135 sensors
- AWS EC2 instance with Ubuntu
- Wi-Fi network
- Arduino IDE or PlatformIO

### ESP32 Setup

1. Install required libraries in Arduino IDE:
   ```
   - DHT sensor library
   - HTTPClient
   - WiFi
   - PubSubClient (for MQTT)
   ```

2. Configure Wi-Fi credentials and AWS endpoint in your ESP32 code

3. Upload the code to ESP32

### AWS EC2 Setup

1. Launch an Ubuntu EC2 instance
2. Install InfluxDB:
   ```bash
   wget -q https://repos.influxdata.com/influxdata-archive_compat.key
   echo '23a1c8836f0afc5ed24e0486339d7cc8f6790b83886c4c96995b88a061c5bb5d influxdata-archive_compat.key' | sha256sum -c
   sudo apt-get update && sudo apt-get install influxdb
   sudo systemctl start influxdb
   ```

3. Install Grafana:
   ```bash
   sudo apt-get install -y software-properties-common
   sudo add-apt-repository "deb https://packages.grafana.com/oss/deb stable main"
   wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -
   sudo apt-get update
   sudo apt-get install grafana
   sudo systemctl start grafana-server
   ```

4. Configure security groups to allow:
   - Port 8086 (InfluxDB)
   - Port 3000 (Grafana)
   - Port 1883 (MQTT, if used)

### Grafana Configuration

1. Access Grafana at `http://<EC2_PUBLIC_IP>:3000`
2. Default credentials: `admin/admin`
3. Add InfluxDB as a data source
4. Create dashboards with panels for temperature, humidity, and air quality

---

## 🚀 Usage

1. Power on the ESP32 device
2. The device will automatically connect to Wi-Fi and start sending data
3. Access Grafana dashboard via browser
4. Monitor real-time and historical air quality data

---


---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📬 Contact

**Portfolio:** [https://kmwhid.netlify.app/](https://kmwhid.netlify.app/)


---

## 🙏 Acknowledgments

- InfluxDB for time-series data storage
- Grafana for powerful visualization
- ESP32 community for IoT development resources
- AWS for cloud infrastructure

---
