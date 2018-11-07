#include "d1_mini.h"
#include "Arduino.h"

#define ERROR_LED D2

/* Logging enabled if pin is 0, i.e. by default */

extern uint8_t global_log_status;

#define LOGGING_PIN D5
#define LOG_ENABLED LOW
#define LOG_DISABLED HIGH

#define IS_LOGGING_ENABLED (global_log_status == LOG_ENABLED)

void setup_logging();
void update_logging_mode();
void print_error(const char *msg);
