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
  Serial.print("----");
}

void setup_osc_server() {
  setup_wifi();
  Udp.begin(UDP_LOCAL_PORT);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
}

void process_udp_packet(void(*callback)(OSCMessage &, int)) {
  OSCBundle bundle;
  int size = Udp.parsePacket();

  // Serial.println("got udp packet");

  if (size > 0) {
    while(size--) {
      // Serial.print("+");
      bundle.fill(Udp.read());
    }

    if (!bundle.hasError()) {
      // Serial.println("route");
      // OSCMessage msg = bundle.getOSCMessage(0);
      // char osc_address[1000];
      // msg.getAddress(osc_address, 0);

      // Serial.print("address: ");
      // Serial.println(osc_address);

      bundle.route("/led", callback);
    } else {
      uint8_t error = bundle.getError();
      print_error("OSC bundle error: ");
      Serial.println(error);
    }
  }
}
