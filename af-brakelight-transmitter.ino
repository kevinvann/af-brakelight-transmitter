
#include <WifiEspNow.h>
#include <WiFi.h>

// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xD8, 0xBC, 0x38, 0xFB, 0x8D, 0x1D};
const int button_pin = 4;

typedef struct struct_message {
  bool is_braking;
} struct_message;

// Create a struct_message called myData
struct_message data;

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);

  // Serial.print("MAC address of this node is ");
  // Serial.println(WiFi.softAPmacAddress());

  pinMode(button_pin, INPUT_PULLUP);

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


  data.is_braking = digitalRead(button_pin) == LOW;

  WifiEspNow.send(PEER, (uint8_t *) &data, sizeof(data));

}