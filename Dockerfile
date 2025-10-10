# Use official Python image
FROM python:3.11-slim

# Set working directory
WORKDIR /code

# Copy requirements
COPY requirement.txt .

# Install dependencies
RUN pip install --upgrade pip && \
    pip install -r requirement.txt

# Copy the rest of the Django project
COPY . .

# Port
EXPOSE 8000

# Start command (optional here, added in docker-compose)
CMD ["python3", "manage.py", "runserver", "0.0.0.0:8000"]
