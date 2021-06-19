#define MSG_BUFFER_SIZE  (50)

#include "actuator.h"


/*MQTT status*/
const char* mqtt_server = "test.mosquitto.org";
const char* outTopic = "IoT/Group1/ToNodeRed";
const char* inTopic = "IoT/Group1/ToNodeMcu";

/*set Client*/
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;
String incomingStr = ""; //for incoming serial data

void sleep_call(){
  delay(5000);
  Serial.println("Entering Deep Sleep Mode for 90 seconds!");
  Serial.println("=======================================");
  ESP.deepSleep(90e6);
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.println("=======================================");
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] :: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // call actuator header 
  String outputToNodeRed = actuate(payload);

  // Publish outputToNodeRed value 
  client.publish(outTopic,&outputToNodeRed[0]);
  Serial.println("## Published to node red");
  sleep_call();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
//      client.publish(outTopic, "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      Serial.println("! Connection Failed :: Retrying in 5 Seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
