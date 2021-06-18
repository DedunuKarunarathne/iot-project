int fan;
int light;
int water_valve;
int battery = 80;
char* wifi = "Dialog";
String stringOutput;
char* outputToNodeRed;

//Define the buffer size
const size_t inputBufferSize = JSON_OBJECT_SIZE(4);
const int outputBufferSize = JSON_OBJECT_SIZE(7);

String actuate( byte* payload) {
   
  DynamicJsonBuffer jsonBuffer(inputBufferSize);
  Serial.println(F("buffer allocated"));

  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
  }
  
  //  Decode JSON/Extract values 
  int temp = root["temp"];
  int humidity = root["humidity"];
  int water_level = root["water_level"];


  // ***** Actuation *****
  Serial.println("######## Actions #######");
  Serial.println(". . . . . . . . . . . . . . . . .");
  
  // Actuate Heater
  if (temp < 1){
      light = 1;
      Serial.println("UV ligth turned on");
      Serial.println(". . . . . . . . . . . . . . . . .");
  }else{
      light = 0;
      Serial.println("UV light turned off");
      Serial.println(". . . . . . . . . . . . . . . . .");   
  };

  // Actuate Fan
  if (humidity > 10){
        fan = 1;
        Serial.println("Fan turned on");
        Serial.println(". . . . . . . . . . . . . . . . .");
  }else{
        fan = 0;
        Serial.println("Fan turned off");
        Serial.println(". . . . . . . . . . . . . . . . .");
  }
      
  // Actuate Water Valve
  if (water_level < 2){
        water_valve = 1;
        Serial.println("Water valve opened");
        Serial.println(". . . . . . . . . . . . . . . . .");
  }else{
        water_valve = 0;
        Serial.println("Water valve closed");
        Serial.println(". . . . . . . . . . . . . . . . .");
  }


  // Allocate output JsonBuffer
  StaticJsonBuffer<outputBufferSize> outputBuffer;

  // Create the out object
  JsonObject& out = outputBuffer.createObject();

  out["wifi"] = wifi; 
  out["battery"] = battery; 
  out["fan"] = fan; 
  out["light"] = light; 
  out["water_valve"] = water_valve; 
  out.printTo(stringOutput);
  
  Serial.println("***");
  Serial.println(stringOutput);
  
  return (stringOutput);
}
