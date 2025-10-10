from django.shortcuts import render
from influxdb_client import InfluxDBClient
from influxdb_client.client.query_api import QueryApi

# InfluxDB config
INFLUX_URL = "http://localhost:8086"
INFLUX_TOKEN = "mysecrettoken123"
INFLUX_ORG = "airquality_org"
INFLUX_BUCKET = "sensor_data"

def dashboard(request):
    try:
        client = InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)
        query_api = client.query_api()

        # Query the latest record
        query = f'''
        from(bucket:"{INFLUX_BUCKET}")
          |> range(start: -1h)
          |> sort(columns: ["_time"], desc: true)
          |> limit(n:1)
        '''
        result = query_api.query(org=INFLUX_ORG, query=query)
        data = {"temperature": 0, "humidity": 0, "air_quality": 0}

        if result and len(result) > 0 and len(result[0].records) > 0:
            record = result[0].records[0]
            data["temperature"] = record.get_value() if record.get_field() == "temperature" else data["temperature"]
            data["humidity"] = record.get_value() if record.get_field() == "humidity" else data["humidity"]
            data["air_quality"] = record.get_value() if record.get_field() == "air_quality" else data["air_quality"]

    except Exception as e:
        print("Error fetching data from InfluxDB:", e)
        data = {"temperature": "N/A", "humidity": "N/A", "air_quality": "N/A"}

    return render(request, "app/index.html", {"data": data})
