#include <WiFiS3.h>          

const char* ssid = "TP-Link_4B2C";           
const char* password = "43545148";   
const char* switchIP = "192.168.0.181";        
const int   switchPort = 9999;          

void setup() {
  Serial.begin(115200);


  Serial.println("\n--- Kasa Switch Serial Controller ---");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi: ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Uno R4 IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Enter '1' to Turn ON, '0' to Turn OFF:");
}

void loop() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read(); 

    if (incomingByte == '1') {
      Serial.println("Action: Turning ON...");
      sendKasaCommand(1);
    } 
    else if (incomingByte == '0') {
      Serial.println("Action: Turning OFF...");
      sendKasaCommand(0);
    }
  }
}

void sendKasaCommand(int state) {
  WiFiClient client;

  if (client.connect(switchIP, switchPort)) {
    String cmd = "{\"system\":{\"set_relay_state\":{\"state\":" + String(state) + "}}}";
    
    String encrypted = encryptKasa(cmd);
    client.print(encrypted);
    client.stop();
    Serial.println("Command sent successfully!");
  } 
  else {
    Serial.println("Connection failed! Check switch IP.");
  }
}

String encryptKasa(String cmd) {
  uint32_t len = cmd.length();
  String result = "";
  

  result += (char)0;
  result += (char)0;
  result += (char)((len >> 8) & 0xFF);
  result += (char)(len & 0xFF);

  uint8_t key = 0xAB;
  for (unsigned int i = 0; i < len; i++) {
    uint8_t a = key ^ cmd[i];
    key = a;
    result += (char)a;
  }
  return result;
}
