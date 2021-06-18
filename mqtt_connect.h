#define MSG_BUFFER_SIZE  (50)

#include "actuator.h"


/*MQTT status*/
const char* mqtt_server = "test.mosquitto.org";
const char* outTopic = "ENTC/170475A/TEST01";
const char* inTopic = "ENTC/170475A/TEST02";

/*set Client*/
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;
String incomingStr = ""; //for incoming serial data

void sleep_call(){
  Serial.println("Entering Deep Sleep Mode for 90 seconds!");
  //ESP.deepSleep(90e6);
}

void callback(char* topic, byte* payload, unsigned int length) {

  // return
  //publish here
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // call actuator header 
  char* outputToNodeRed = actuate(payload);

  Serial.println(outputToNodeRed);
  
  // Publish outputToNodeRed value 
  client.publish(outTopic,outputToNodeRed );
  Serial.println("Published to node red");
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
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
