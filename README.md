# 🌐 IoT-Based Air Quality Monitoring System

This project is a **real-time IoT Air Quality Monitoring System** that collects sensor data from an ESP32 and visualizes it through **Grafana** using **InfluxDB** as the time-series database.
It provides live dashboards and historical analysis for environmental data such as temperature, humidity, and air quality index.

> 🟢 **Note:** The entire system (InfluxDB + Grafana + Django backend) is **deployed on AWS EC2**, allowing remote access and scalability for multiple IoT devices.

---

## 🚀 System Overview

The system enables **continuous monitoring** of air quality through an IoT device (ESP32) equipped with sensors.
Data is sent over Wi-Fi to a **cloud-hosted InfluxDB instance**, which stores all readings for further visualization in **Grafana** dashboards.
The backend is hosted on **AWS**, ensuring high availability and secure connectivity between devices and the dashboard.

---

## 🧠 How the System Works

### 1. **Data Collection (ESP32)**

* The ESP32 board reads sensor data from modules like:

  * **DHT11/DHT22** → Temperature and Humidity
  * **MQ135** → Air Quality / Gas concentration
* The sensor readings are converted into structured JSON data.
* The ESP32 connects to Wi-Fi and sends this data to the AWS-hosted InfluxDB endpoint.

### 2. **Data Transmission**

* Data is sent using the **HTTP POST** or **MQTT** protocol to the AWS server’s InfluxDB endpoint.
* Example data payload:

```json
{
  "temperature": 27.4,
  "humidity": 56.3,
  "air_quality": 135
}
```

### 3. **Data Storage (InfluxDB on AWS)**

The incoming data is written to **InfluxDB**, a time-series database running on an AWS EC2 instance.
Each sensor reading is stored with a timestamp, enabling time-based querying and trend analysis.

### 4. **Data Visualization (Grafana on AWS)**

Grafana, hosted on the same AWS EC2 server, connects to InfluxDB as a data source.
Dashboards are created to visualize:

* Real-time temperature, humidity, and air quality levels
* Historical trends and variations
* Threshold-based color-coded gauges (e.g., green, yellow, red)

---

## 🏗️ System Architecture

```text
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

☁️ **Cloud Deployment:**
The entire backend stack (InfluxDB + Grafana + Django API) runs on an AWS EC2 instance, allowing ESP32 devices to push data remotely and Grafana dashboards to be accessed via a public IP.

---

## 📊 Dashboard Example

A Grafana dashboard can display:

* Live gauges for Temperature, Humidity, Air Quality
* Historical graphs showing hourly or daily variation
* Color-coded alert zones (e.g., Green < 20°C, Yellow < 30°C, Red > 30°C)
* Real-time data refresh using the InfluxDB time-series connection

---

## 🔄 Data Flow Visualization


+-------------------+ +-------------------+ +-------------------+ +-------------------+
| ESP32 Sensor Node | ---> | Wi-Fi Local Network| ---> | InfluxDB (AWS) | ---> | Grafana (AWS) |
| Reads & formats | | Transmits Data | | Stores time-series| | Visualizes data |
| sensor data | | | | data | | on dashboard |
+-------------------+ +-------------------+ +-------------------+ +-------------------+

Step 1: ESP32 reads & formats sensor data
|
Step 2: Data sent over Wi-Fi
|
Step 3: InfluxDB stores the time-series data
|
Step 4: Grafana reads data and visualizes it on dashboards

---

## 🧩 Key Features

* 🌍 **AWS-Hosted Cloud Deployment**
* ⚡ Real-time monitoring of environmental data
* 📈 Time-based trend visualization
* 🔒 Secure and scalable architecture
* 🧱 Modular system — easily extendable with more sensors
* 🧠 Open-source and IoT-friendly design

---

## 🧾 License

This project is open-source and available under the **MIT License**.

---

## 📬 Contact

🌐 [https://kmwhid.netlify.app/](https://kmwhid.netlify.app/)
