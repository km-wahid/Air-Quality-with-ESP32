from django.urls import path, include
from app.views import dashboard
urlpatterns = [
    path('', dashboard, name='dashboard')
]