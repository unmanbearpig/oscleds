#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "wifi_network.h"
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#define UDP_LOCAL_PORT 8888

void setup_wifi();
void setup_osc_server();
void led_msg(OSCMessage &msg, int offset);
void process_udp_packet(void(*linear_callback)(OSCMessage &, int), void(*log_callback)(OSCMessage &, int));
void check_wifi_status();
