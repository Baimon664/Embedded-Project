#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "";
const char* mqtt_username = "";
const char* mqtt_password = "";
char msg[100];

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial){
    ;
  }
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.hostname("ESP-host");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.subscribe("@msg/led");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting NETPIE2020 connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("Netpie 2020 connect");
      client.subscribe("@msg/led");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      reconnect();
  }
  client.loop();
  char data=Serial.read();
  //Serial.println(data);
  if(data=='n'){
    //Serial.println(data);
    client.publish("@msg/status", "one");
  }
  else if(data=='f'){
    client.publish("@msg/status", "offe");
  }
  else if(data=='e'){
    client.publish("@msg/status", "offd");
  }
  else if(data=='k'){
    client.publish("@msg/status", "ond");
  }
  delay(250);
}

void callback(char* topic, byte* payload, unsigned int length) {
    //Serial.println("Callback");
//    Serial.print("Message arrive [");
//    Serial.print(topic);
//    Serial.print("]");
    String message;
    char n='n';
    char f='f';
    for (int i = 0; i < length; i++) {
        message = message + (char)payload[i];
    }
    //Serial.println(message);
    if (message == "LEDON") {
        Serial.write('p');
    }
    else if(message == "LEDOFF"){
        Serial.write('u');
    }
    else if(message == "MICOFF"){
        Serial.write('j');
    }
    else if(message == "MICON"){
      Serial.write('i');
    }
}
