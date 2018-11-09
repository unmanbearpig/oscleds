#include "Arduino.h"

typedef struct {
  uint8_t cmd;
  uint16_t led_9_brightness;
  uint16_t led_10_brightness;
} Brightness;

extern Brightness currentBrightness;

void setLed(int led, uint16_t value);

void updateLedBrightness(Brightness *newBrightness);
