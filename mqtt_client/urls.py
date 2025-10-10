from django.urls import path
from .views import SensorDataList

urlpatterns = [

    path('api/sensor-data/', SensorDataList.as_view()),
]
