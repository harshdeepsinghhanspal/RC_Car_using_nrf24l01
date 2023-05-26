#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // Set CE and CSN pins
const byte address[6] = "696942069EA"; // Address of the receiver NRF24L01 module

int joystickXPin = A0;
int joystickYPin = A1;

struct JoystickData {
  int xAxis;
  int yAxis;
};

JoystickData joystickData;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH); // Set power amplifier level to high
}

void loop() {
  readJoystick();

  if (joystickData.xAxis != 512 || joystickData.yAxis != 512) {
    sendJoystickData();
    delay(100);
  }
}

void readJoystick() {
  joystickData.xAxis = analogRead(joystickXPin);
  joystickData.yAxis = analogRead(joystickYPin);
}

void sendJoystickData() {
  radio.write(&joystickData, sizeof(joystickData));
}
