#include "i2c_leds.h"
#include "Wire.h"
#include "status.h"

void setup_i2c_leds() {
  Wire.begin(LOCAL_SDA_PIN, LOCAL_SCL_PIN);
  Wire.setClockStretchLimit(40000);
}

uint8_t set_led_brightness(uint8_t dev_addr, LED_VALUE led_1_value, LED_VALUE led_2_value) {
  Wire.beginTransmission(dev_addr);

  LED_I2C_MESSAGE msg = { I2C_MAGIC_NUM, led_1_value, led_2_value };

  Wire.write((uint8_t *) &msg, sizeof(msg));
  uint8_t result = Wire.endTransmission();

  if (IS_LOGGING_ENABLED) {
    if (result == 0) {
      Serial.print(".");
    } else {
      Serial.print(result, DEC);
    }
  }

  return result;
}

void update_led_dev_state(DEV dev, LED_STATE *led_state) {
  if (dev == DEV_1) {
    set_led_brightness(dev, led_state->dev_1_led_1, led_state->dev_1_led_2);
  } else if (dev == DEV_2) {
    set_led_brightness(dev, led_state->dev_2_led_1, led_state->dev_2_led_2);
  }

  // ignore otherwise, error should be reported by this point
}

void print_led_state(LED_STATE *led_state) {
  Serial.println("led state: ");
  Serial.print(led_state->dev_1_led_1, DEC);
  Serial.print(" | ");
  Serial.print(led_state->dev_1_led_2, DEC);
  Serial.print(" | ");
  Serial.print(led_state->dev_2_led_1, DEC);
  Serial.print(" | ");
  Serial.println(led_state->dev_2_led_2, DEC);
  Serial.println("----------------");
}

void set_all_leds(LED_STATE *led_state, LED_VALUE led_value) {
  led_state->dev_1_led_1 = led_value;
  led_state->dev_1_led_2 = led_value;
  led_state->dev_2_led_1 = led_value;
  led_state->dev_2_led_2 = led_value;
}

void change_all_leds(LED_STATE *led_state, LED_VALUE led_value) {
  set_all_leds(led_state, led_value);
  update_led_dev_state(DEV_1, led_state);
  update_led_dev_state(DEV_2, led_state);
}

DEV get_led_dev_id(unsigned int led_num) {
  if (led_num == 1 || led_num == 2) {
    return DEV_1;
  } else if(led_num == 3 || led_num == 4) {
    return DEV_2;
  } else {
    print_error("Invalid LED number:");
    Serial.println(led_num, DEC);
    return 0;
  }
}

LED_VALUE *get_p_led_value_from_state(LED_STATE *led_state, unsigned int led_num) {
  if (led_num == 1) {
    return &led_state->dev_1_led_1;
  } else if(led_num == 2) {
    return &led_state->dev_1_led_2;
  } else if(led_num == 3) {
    return &led_state->dev_2_led_1;
  } else if(led_num == 4) {
    return &led_state->dev_2_led_2;
  } else {
    print_error("Invalid LED number:");
    Serial.println(led_num);
    return 0;
  }
}

void set_led_value(LED_STATE *led_state, unsigned int led_num, LED_VALUE led_value) {
  LED_VALUE *target_value = get_p_led_value_from_state(led_state, led_num);

  (*target_value) = led_value;
}

LED_VALUE get_led_value(LED_STATE *led_state, unsigned int led_num) {
  return *get_p_led_value_from_state(led_state, led_num);
}

void change_single_led_value(LED_STATE *led_state, unsigned int led_num , LED_VALUE value) {
  // Serial.print("changing value of led ");
  // Serial.print(led_num);
  // Serial.print(" to value ");
  // Serial.println(value);

  set_led_value(led_state, led_num, value);
  update_led_dev_state(get_led_dev_id(led_num), led_state);
}
