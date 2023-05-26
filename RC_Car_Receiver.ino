#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

RF24 radio(7, 8); // Set CE and CSN pins
const byte address[6] = "696942069EA"; // Address of the transmitter NRF24L01 module

int motor1Pin1 = 2; // L298N motor driver input 1
int motor1Pin2 = 3; // L298N motor driver input 2
int motor2Pin1 = 4; // L298N motor driver input 3
int motor2Pin2 = 5; // L298N motor driver input 4

struct JoystickData {
  int xAxis;
  int yAxis;
};

JoystickData joystickData;

void setup() {
  Serial.begin(9600);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_HIGH); // Set power amplifier level to high
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&joystickData, sizeof(joystickData));
    driveMotors();
  }
}

void driveMotors() {
  int speed = map(joystickData.yAxis, 0, 1023, -255, 255);
  int rotation = map(joystickData.xAxis, 0, 1023, -255, 255);

  int leftSpeed = speed + rotation;
  int rightSpeed = speed - rotation;

  if (leftSpeed >= 0) {
    analogWrite(motor1Pin1, leftSpeed);
    analogWrite(motor1Pin2, 0);
  } else {
    analogWrite(motor1Pin1, 0);
    analogWrite(motor1Pin2, -leftSpeed);
  }

  if (rightSpeed >= 0) {
    analogWrite(motor2Pin1, rightSpeed);
    analogWrite(motor2Pin2, 0);
  } else {
    analogWrite(motor2Pin1, 0);
    analogWrite(motor2Pin2, -rightSpeed);
  }
}
