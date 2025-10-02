#include <DHT.h>

// 센서 핀 정의
#define DHTPIN 2
#define DHTTYPE DHT11
#define LIGHT_SENSOR_PIN A0
#define SOIL_MOISTURE_PIN A1

const char* ssid = "ms25";
const char* password = "00000001";
const char* host = "43.201.68.168";  
const int port = 8080;               

String DEVICE_ID = "arduino-mega-01";

DHT dht(DHTPIN, DHTTYPE);
boolean wifiConnected = false;

void setup() {
  Serial.begin(9600);  // ESP8266과 통신
  delay(2000);
  dht.begin();
  delay(2000);  // DHT 안정화 시간
  initESP8266();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // 아날로그 값을 백분율(%)로 변환
  float light_level = analogRead(LIGHT_SENSOR_PIN) / 1023.0 * 100.0;
  float soil_moisture = analogRead(SOIL_MOISTURE_PIN) / 1023.0 * 100.0;

  sendSensorDataToServer(temperature, humidity, light_level, soil_moisture);
  delay(1800000);
}

boolean sendCommand(String command, String expectedResponse, int timeout) {
  Serial.println(">> " + command);
  Serial.print(command + "\r\n");

  long startTime = millis();
  String response = "";
  while (millis() - startTime < timeout) {
    while (Serial.available()) {
      char c = Serial.read();
      response += c;
      if (response.indexOf(expectedResponse) != -1) {
        return true;
      }
    }
  }
  return false;
}

void initESP8266() {
  sendCommand("AT", "OK", 2000);
  sendCommand("AT+CWMODE=1", "OK", 2000);

  String connectCmd = "AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"";
  if (sendCommand(connectCmd, "OK", 15000)) {
    wifiConnected = true;
  } else {
    wifiConnected = false;
  }
}

void sendSensorDataToServer(float temperature, float humidity, float light_level, float soil_moisture) {
  if (!wifiConnected) return;

  String cmd = "AT+CIPSTART=\"TCP\",\"" + String(host) + "\"," + String(port);
  if (sendCommand(cmd, "OK", 5000)) {
    // JSON 데이터 구성
    String jsonData = "{";
    jsonData += "\"deviceId\":\"" + DEVICE_ID + "\",";                       
    jsonData += "\"temperature\":" + String(temperature, 2) + ",";          
    jsonData += "\"humidity\":" + String(humidity, 2) + ",";                
    jsonData += "\"light_level\":" + String(light_level, 1) + ",";          
    jsonData += "\"soil_moisture\":" + String(soil_moisture, 1);
    jsonData += "}";

    // HTTP 요청 구성
    String httpRequest = "POST /api/sensor-data HTTP/1.1\r\n";
    httpRequest += "Host: " + String(host) + "\r\n";
    httpRequest += "Content-Type: application/json\r\n";
    httpRequest += "Connection: close\r\n";
    httpRequest += "Content-Length: " + String(jsonData.length()) + "\r\n\r\n";
    httpRequest += jsonData;

    if (sendCommand("AT+CIPSEND=" + String(httpRequest.length()), ">", 5000)) {
      Serial.print(httpRequest);
    }

    delay(3000);
    sendCommand("AT+CIPCLOSE", "OK", 1000);
  }
}
