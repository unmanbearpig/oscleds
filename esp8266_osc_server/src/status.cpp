#include "Arduino.h"
#include "status.h"

void print_error(char *msg) {
  digitalWrite(ERROR_LED, HIGH);
  Serial.println(msg);
}
