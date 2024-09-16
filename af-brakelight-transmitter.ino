
#include <WifiEspNow.h>
#include <WiFi.h>
#include <Debounce.h>

// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xD8, 0xBC, 0x38, 0xFB, 0x8D, 0x1D};
const int button_pin = 4;
Debounce button(button_pin, 50, true);
int button_state = LOW;
int last_button_state = LOW;

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(button_pin, INPUT_PULLUP);

  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);

  // Serial.print("MAC address of this node is ");
  // Serial.println(WiFi.softAPmacAddress());

  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }

  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }
}

void loop() {

  button_state = button.read();
  if (button_state != last_button_state) {
    char msg[60];
      int len = snprintf(msg, sizeof(msg), "hello ESP-NOW from %s at %lu",
                     WiFi.softAPmacAddress().c_str(), millis());
      WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len);
  }

  last_button_state = button_state;
}