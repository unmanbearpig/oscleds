#include "Arduino.h"
#include "pwm_16.h"
#include <Wire.h>
#include <WireData.h>
#include <status.h>
#include "led.h"

const uint8_t logging_pin = 5;
const uint8_t i2c_address_pin = 6;

int led = LED_BUILTIN;

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

uint8_t getI2cAddress() {
  pinMode(i2c_address_pin, INPUT_PULLUP);

  if (digitalRead(i2c_address_pin) == HIGH) {
    return 8;
  } else {
    return 12;
  }
}

void setupI2c(uint8_t i2c_address) {
  Wire.begin(i2c_address);      // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
}

void setup() {
  Serial.begin(9600);           // start serial for output
  Serial.println("Pro Mini started! on port 12");
  setup_logging();
  setupI2c(getI2cAddress());
  setupPWM16();
  analogWrite16(9, 0xffff);
  analogWrite16(10, 0xffff);
}

uint16_t loop_index = 0;

void print_current_brightness() {
  Serial.print("LED 9: ");
  Serial.println(currentBrightness.led_9_brightness, HEX);

  Serial.print("LED 10: ");
  Serial.println(currentBrightness.led_10_brightness, HEX);
}

uint16_t prevUpdateCycle = 0;

void loop() {
  loop_index++;

  if (loop_index % 1000000 == 0) {
    update_logging_mode();

    if (IS_LOGGING_ENABLED) {
      print_current_brightness();
    }
  }
}

void receiveEvent(int byteCount) {
  Brightness receivedBrightness;

  if (IS_LOGGING_ENABLED) {
    Serial.print(byteCount);
    Serial.println(" bytes");
  }

  wireReadData(receivedBrightness);

  if (receivedBrightness.cmd == 123) {
    if (IS_LOGGING_ENABLED) {
      Serial.print(".");
    }
    memcpy(&newBrightness, &receivedBrightness, sizeof(Brightness));
    updateLedBrightness(&newBrightness);
  } else {
    if (IS_LOGGING_ENABLED) {
      Serial.print("expected magic number but got:");
      Serial.print(receivedBrightness.cmd, HEX);
    }
  }
}
