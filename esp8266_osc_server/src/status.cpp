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

void print_error(const char *msg) {
  if (IS_LOGGING_ENABLED) {
    return;
  }

  digitalWrite(ERROR_LED, HIGH);
  Serial.println(msg);
}
