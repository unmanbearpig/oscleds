#include "Arduino.h"
#include "Wire.h"
#include "i2c_leds.h"
#include "osc_server.h"
#include "status.h"

LED_STATE global_led_state = { 0, 0, 0, 0 };

void setup() {
  // initialize LED digital pin as an output.
  Serial.begin(9600);
  Serial.print("9600");
  pinMode(LED_BUILTIN, OUTPUT);
  setup_i2c_leds();
  setup_osc_server();
}

void led_msg(OSCMessage &msg, int offset) {
  LED_VALUE value;
  if (msg.isInt(0)) {
    value = msg.getInt(0);

    // Serial.print("got int ");
    // Serial.println(value);
  } else if (msg.isFloat(0)) {
    float float_value = msg.getFloat(0);
    value = float_value * 65535;

    // Serial.print("got float ");
    // Serial.print(float_value);
    // Serial.print(" int = ");
    // Serial.println(value);
  } else {
    print_error("unknown type");
  }

  if (msg.fullMatch("/1", offset)) {
    // Serial.println("led 1");
    change_single_led_value(&global_led_state, 1, value);
  } else if (msg.fullMatch("/2", offset)) {
    // Serial.println("led 2");
    change_single_led_value(&global_led_state, 2, value);
  } else if (msg.fullMatch("/3", offset)) {
    // Serial.println("led 3");
    change_single_led_value(&global_led_state, 3, value);
  } else if (msg.fullMatch("/4", offset)) {
    // Serial.println("led 4");
    change_single_led_value(&global_led_state, 4, value);
  } else {
    print_error("unknown dev path");
  }
}

void loop() {
  process_udp_packet(led_msg);
}
