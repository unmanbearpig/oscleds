#include "led.h"
#include <status.h>
#include "pwm_16.h"

void setLed(int led, uint16_t value) {
  if (IS_LOGGING_ENABLED) {
    Serial.print("+");
  }
  analogWrite16(led, value);
}

void updateLedBrightness(Brightness *newBrightness) {
  if (memcmp(newBrightness, &currentBrightness, sizeof(Brightness)) == 0) {
    return;
  }

  setLed(9, newBrightness->led_9_brightness);
  setLed(10, newBrightness->led_10_brightness);

  memcpy(&currentBrightness, newBrightness, sizeof(Brightness));
}
