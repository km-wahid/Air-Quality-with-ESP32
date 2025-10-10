from rest_framework.views import APIView
from rest_framework.response import Response
from .models import SensorData
from .serializers import SensorDataSerializer

class SensorDataList(APIView):
    def get(self, request):
        data = SensorData.objects.order_by('-timestamp')[:50]
        serializer = SensorDataSerializer(data, many=True)
        return Response(serializer.data)  # <-- Must return serialized data

# views.py

