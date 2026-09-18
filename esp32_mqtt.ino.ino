#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Cấu hình Wi-Fi và MQTT Broker
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;

// Định nghĩa các Topic MQTT
const char* topic_status  = "smartparking/status/slots/node_01";
const char* topic_control = "smartparking/control/barrier/node_01";
const char* topic_lwt     = "smartparking/health/lwt";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32_Park_Client_" + String(random(0xffff), HEX);
    
    // Cấu hình Last Will and Testament (LWT) tại topic health/lwt
    const char* lwt_payload = "{\"node_id\":\"node_01\",\"status\":\"OFFLINE\"}";
    
    if (client.connect(clientId.c_str(), topic_lwt, 1, true, lwt_payload)) {
      // Đăng ký nhận lệnh điều khiển Barrier
      client.subscribe(topic_control, 1);
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Mô phỏng gửi dữ liệu định kỳ kèm timestamp (đo độ trễ T1)
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  
  if (now - lastMsg > 5000) { // Mỗi 5 giây gửi 1 lần
    lastMsg = now;

    // Đóng gói JSON Payload
    StaticJsonDocument<200> doc;
    doc["slot_id"] = "A01";
    doc["occupied"] = true;
    doc["timestamp"] = now;

    char buffer[256];
    serializeJson(doc, buffer);

    // Bật Retained flag (true) và gửi dữ liệu lên Broker
    client.publish(topic_status, buffer, true);
    Serial.print("[MQTT] Sent Slots Status (T1 = ");
    Serial.print(now);
    Serial.println(" ms)");
  }
}