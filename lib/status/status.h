#include "Arduino.h"

/* Logging enabled if pin is 0, i.e. by default */

extern uint8_t error_led;
extern uint8_t global_log_status;
extern const uint8_t logging_pin;

#define LOG_ENABLED LOW
#define LOG_DISABLED HIGH

#define IS_LOGGING_ENABLED (global_log_status == LOG_ENABLED)

void setup_logging();
void update_logging_mode();
void print_error(const char *msg);
void log_msg(const char *msg);
