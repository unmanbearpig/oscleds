#include "Arduino.h"
#include "status.h"

uint8_t global_log_status = LOG_ENABLED;

void setup_logging() {
  pinMode(LOGGING_PIN, INPUT);
  update_logging_mode();
}

void update_logging_mode() {
  global_log_status = digitalRead(LOGGING_PIN);
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
