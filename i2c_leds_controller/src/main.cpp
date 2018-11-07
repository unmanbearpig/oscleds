#include "Arduino.h"
#include "pwm_16.h"
#include <Wire.h>
#include <WireData.h>

#define I2C_ADDRESS 12
int led = LED_BUILTIN;
char led_value = 0xff;
char target_led_value = 0xff;
int is_waiting_for_led_value = 0;

typedef struct {
  uint8_t cmd;
  uint16_t led_9_brightness;
  uint16_t led_10_brightness;
} Brightness;

Brightness currentBrightness = {
  0x00,
  0xffff,
  0xffff
};

Brightness newBrightness = {
  0x00,
  0xffff,
  0xffff
};

void receiveEvent(int byteCount);

void setupI2c() {
  Wire.begin(I2C_ADDRESS);      // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
}

void setup() {
  Serial.begin(9600);           // start serial for output
  Serial.println("Pro Mini started! on port 12");
  setupI2c();
  setupPWM16();
  analogWrite16(9, 0xffff);
  analogWrite16(10, 0xffff);
}

void setLed(int led, uint16_t value) {
  // Serial.print("Setting LED ");
  // Serial.print(led);
  // Serial.print(" to ");
  // Serial.println(value, HEX);
  Serial.print("+");
  analogWrite16(led, value);
}

void updateLed() {
  if (memcmp(&newBrightness, &currentBrightness, sizeof(Brightness)) == 0) {
    return;
  }

  setLed(9, newBrightness.led_9_brightness);
  setLed(10, newBrightness.led_10_brightness);

  memcpy(&currentBrightness, &newBrightness, sizeof(Brightness));
}

int loop_index = 0;

void loop() {
  loop_index++;

  // delay(10);
  if (loop_index % 40000 == 0) {

    Serial.print("LED 9: ");
    Serial.println(currentBrightness.led_9_brightness, HEX);

    Serial.print("LED 10: ");
    Serial.println(currentBrightness.led_10_brightness, HEX);
  }

  if (loop_index % 100 == 0) {
    updateLed();
  }
}


void receiveEvent(int byteCount) {
  Brightness receivedBrightness;

  // Serial.print("Received ");
  Serial.print(byteCount);
  Serial.println(" bytes");

  wireReadData(receivedBrightness);

  if (receivedBrightness.cmd == 123) {
    // Serial.println("got magic number");
    Serial.print(".");
    memcpy(&newBrightness, &receivedBrightness, sizeof(Brightness));
  } else {
    // Serial.print("expected magic number but got:");
    Serial.print(receivedBrightness.cmd, HEX);
  }
}
