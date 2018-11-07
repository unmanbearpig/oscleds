#include "Arduino.h"
#include "d1_mini.h"

#define DEV uint8_t

#define DEV_1 0x08
#define DEV_2 0x0c

#define I2C_MAGIC_NUM 123

#define LED_VALUE uint16_t

#define LOCAL_SCL_PIN D5
#define LOCAL_SDA_PIN D6

typedef struct __attribute__((__packed__)) {
  uint8_t magic_number;
  LED_VALUE led_1_value;
  LED_VALUE led_2_value;
} LED_I2C_MESSAGE;

typedef struct {
  LED_VALUE dev_1_led_1;
  LED_VALUE dev_1_led_2;
  LED_VALUE dev_2_led_1;
  LED_VALUE dev_2_led_2;
} LED_STATE;

uint8_t set_led_brightness(uint8_t dev_addr, LED_VALUE led_1_value, LED_VALUE led_2_value);

void print_led_state(LED_STATE *led_state);

void update_led_state(LED_STATE *led_state);

void set_all_leds(LED_STATE* led_state, LED_VALUE led_value);

void change_single_led_value(LED_STATE *led_state, unsigned int led_num , LED_VALUE value);

void change_all_leds(LED_STATE *led_state, LED_VALUE led_value);

void setup_i2c_leds();
