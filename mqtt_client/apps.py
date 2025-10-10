from django.apps import AppConfig
import threading

class MqttClientConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'mqtt_client'

    def ready(self):
        # Import inside ready(), so Django models are ready
        from .mqtt_utlis import start_mqtt_listener

        # Run listener in a separate thread
        thread = threading.Thread(target=start_mqtt_listener)
        thread.daemon = True
        thread.start()
