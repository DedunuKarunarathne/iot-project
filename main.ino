#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "soft_ap.h"
#include "eeprom_rw.h"
#include "wifi_connect.h"
#include "mqtt_connect.h"

/*WiFi status*/
bool wifi_connected = false;
String ssid;
String password;


void setup(){
  Serial.begin(115200);
  EEPROM.begin(512);
  //clear_eeprom();
  delay(2000);
  //delay(5000);
  Serial.println("=======================================");
  if (credentials_stored()){
    Serial.println("RETRIEVING CREDENTIALS FROM EEPROM ...");
//    Serial.print("SSID RECEIVED : ");
//    Serial.print(read_eeprom(0));
//    Serial.println();
//    Serial.print("PASSWORD RECEIVED : ");
//    Serial.print(read_eeprom(1));
//    Serial.println();
    if (setup_wifi(read_eeprom(0), read_eeprom(1))){
      client.setServer(mqtt_server, 1883);
      client.setCallback(callback);
      wifi_connected = true;
    }
  } 
}
 
void loop(){

  if (!wifi_connected){
    Serial.println("=======================================");
    Serial.println("####### Wifi Disconnected");
    Serial.println("####### Starting Access Point ...");
    start_softAP();
    while (true){
      delay(5000);
      ssid = get_ssid();
      password = get_password();
      if (ssid != "" && password != ""){
        if (setup_wifi(ssid, password)){
          delay(5000);
          write_eeprom(ssid, password);
          client.setServer(mqtt_server, 1883);
          client.setCallback(callback);
          wifi_connected = true;
        } 
        else{
          reset_credentials();
        }
        Serial.println("=======================================");
        break;
      }
    }  
  }


  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  
}
