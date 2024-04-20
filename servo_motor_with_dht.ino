#include <Servo.h>
#include <DHT.h>

#define DHTPIN D4       // Pin connected to the DHT sensor
#define DHTTYPE DHT11   // Change this to DHT22 if you're using that type of sensor
#define SERVO_PIN D1    // Pin connected to the servo motor
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

void setup() {
  Serial.begin(115200);
  delay(100);

  dht.begin();
  servo.attach(SERVO_PIN);
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

  // Map temperature to servo angle
  int servoAngle = map(temperature, 0, 40, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  servo.write(servoAngle);
  delay(2000); // Delay to allow servo to move to the desired position

  // Move the servo back and forth
  for (int angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle++) {
    servo.write(angle);
    delay(15); // Adjust the delay as needed for the speed of rotation
  }
  delay(2000); // Delay before reversing the rotation
  for (int angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle--) {
    servo.write(angle);
    delay(15); // Adjust the delay as needed for the speed of rotation
  }
  delay(2000); // Delay before next iteration
}
