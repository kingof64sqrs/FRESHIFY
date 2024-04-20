#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D4       // Pin connected to the DHT sensor
#define DHTTYPE DHT11   // Change this to DHT22 if you're using that type of sensor
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
#define SERVER_URL "YourServerURL"

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(100);

  dht.begin();
  connectToWiFi();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  sendToServer(temperature, humidity);
  delay(2000); // Delay before next reading
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void sendToServer(float temperature, float humidity) {
  if (client.connect(SERVER_URL, 80)) {
    Serial.println("Connected to server");
    String url = "/update?temperature=" + String(temperature) + "&humidity=" + String(humidity);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + SERVER_URL + "\r\n" +
                 "Connection: close\r\n\r\n");
    Serial.println("Data sent to server");
  } else {
    Serial.println("Connection to server failed");
  }
}
