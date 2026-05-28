FROM python:3.9-slim

WORKDIR /app

COPY requirements.txt .
RUN pip install -r requirements.txt

COPY demo3/web/app.py ./
COPY demo3/web/templates ./templates

EXPOSE 5000

CMD ["python", "app.py"]