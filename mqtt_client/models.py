from django.db import models

class SensorData(models.Model):
    temperature = models.FloatField()
    humidity = models.FloatField()
    gas_level = models.FloatField(null=True, blank=True)
    timestamp = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"{self.timestamp}: {self.temperature}°C, {self.humidity}%, {self.gas_level}"
