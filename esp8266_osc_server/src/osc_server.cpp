#include "osc_server.h"
#include "status.h"

WiFiUDP Udp;

void setup_wifi() {
  Serial.print("Using WiFi network: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("----");
}

void check_wifi_status() {
  if (WiFi.status() != WL_CONNECTED) {
    log_msg("WiFi is not connected. Reconnecting.");
    setup_wifi();
  }
}

void setup_osc_server() {
  setup_wifi();
  Udp.begin(UDP_LOCAL_PORT);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
}

void process_udp_packet(void(*linear_callback)(OSCMessage &, int), void(*log_callback)(OSCMessage &, int)) {
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while(size--) {
      uint8_t byte = Udp.read();
      bundle.fill(byte);

      if (IS_LOGGING_ENABLED) {
        Serial.print(byte, HEX);
      }
    }

    if (!bundle.hasError()) {
      if (IS_LOGGING_ENABLED) {
        Serial.println("route");
        OSCMessage msg = bundle.getOSCMessage(0);
        char osc_address[1000];
        msg.getAddress(osc_address, 0);

        Serial.print("address: ");
        Serial.println(osc_address);
      }

      bundle.route("/led", linear_callback);
      bundle.route("/log", log_callback);
    } else {
      uint8_t error = bundle.getError();

      if (IS_LOGGING_ENABLED) {
        Serial.print("OSC bundle error: ");
        Serial.println(error);
      }
    }
  }
}
