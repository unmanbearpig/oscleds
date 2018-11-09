#include "Arduino.h"
#include "Wire.h"
#include "i2c_leds.h"
#include "osc_server.h"
#include <status.h>
#include "d1_mini.h"

uint8_t error_led = D2;
const uint8_t logging_pin = D7;
LED_STATE global_led_state = { 0, 0, 0, 0 };

void process_single_led_route(OSCMessage &msg, int offset, LED_VALUE value) {
  if (msg.fullMatch("/1", offset)) {
    change_single_led_value(&global_led_state, 1, value);
  } else if (msg.fullMatch("/2", offset)) {
    change_single_led_value(&global_led_state, 2, value);
  } else if (msg.fullMatch("/3", offset)) {
    change_single_led_value(&global_led_state, 3, value);
  } else if (msg.fullMatch("/4", offset)) {
    change_single_led_value(&global_led_state, 4, value);
  } else {
    print_error("unknown dev path");
  }
}

// true for success
bool parse_msg_int_value(OSCMessage &msg, LED_VALUE *result) {
  if (msg.isInt(0)) {
    (*result) = msg.getInt(0);
    return true;
  } else if (msg.isFloat(0)) {
    float float_value = msg.getFloat(0);
    (*result) = float_value * 65535;
    return true;
  } else {
    print_error("unknown type");
    return false;
  }
}

bool parse_msg_float_value(OSCMessage &msg, float *result) {
  if (msg.isInt(0)) {
    LED_VALUE i = msg.getInt(0);
    (*result) = i / 65535.0;
    return true;
  } else if (msg.isFloat(0)) {
    (*result) = msg.getFloat(0);
    return true;
  } else {
    print_error("unknown type");
    return false;
  }
}

void linear_led_msg(OSCMessage &msg, int offset) {
  LED_VALUE value = 0;

  if (!parse_msg_int_value(msg, &value)) {
    return;
  }

  process_single_led_route(msg, offset, value);
}

void log_led_msg(OSCMessage &msg, int offset) {
  float fvalue = 0;

  if (!parse_msg_float_value(msg, &fvalue)) {
    return;
  }

  LED_VALUE ivalue = pow(fvalue, 4) * 65535;
  process_single_led_route(msg, offset, ivalue);
}

void setup() {
  // initialize LED digital pin as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  setup_logging();
  setup_i2c_leds();
  setup_osc_server();
}

uint16_t loop_counter = 0;

void loop() {
  loop_counter += 1;

  if (loop_counter % 500000 == 0) {
    update_logging_mode();
    check_wifi_status();
  }

  process_udp_packet(linear_led_msg, log_led_msg);
}
