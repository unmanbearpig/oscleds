#include "Arduino.h"
#include "status.h"

uint8_t global_log_status = LOG_ENABLED;

void print_log_status(const uint8_t status) {
  if (status == LOG_ENABLED) {
    Serial.println("Logging enabled");
  } else {
    Serial.println("Logging disabled");
  }
}

void setup_logging() {
  pinMode(logging_pin, INPUT_PULLUP);
  Serial.print("Disable logging by pulling down pin ");
  Serial.println(logging_pin);
  update_logging_mode();
  print_log_status(global_log_status);
}

void update_logging_mode() {
  if (IS_LOGGING_ENABLED) {
    Serial.println("updating logging mode");
  }

  uint8_t new_status = digitalRead(logging_pin);

  if (new_status != global_log_status) {
    print_log_status(new_status);
  }

  global_log_status = new_status;
}

void log_msg(const char *msg) {
  if (!IS_LOGGING_ENABLED) {
    return;
  }

  Serial.println(msg);
}

void print_error(const char *msg) {
  if (!IS_LOGGING_ENABLED) {
    return;
  }

  digitalWrite(error_led, HIGH);
  Serial.println(msg);
}
