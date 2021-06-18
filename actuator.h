/* Parameters sent to the node red
 *  
 */
int battery = 80;
int fan;
int light;
int water_valve;

/*
 * Randomly generated values
 */
 int water_level  = random(0,4);
 float water_temp = 15 + random(0,100)/10.0;
 float ph_level   = 6 + random(1,200)/100.0;

/*
 * Parameters received from node red
 */
float temp;
float humidity;
float pm10;
float wind_speed;
float dew_point;

String outputToNodeRed;
  
/*
 * Buffers for json objects
 */
const size_t inputBufferSize = JSON_OBJECT_SIZE(10);
const int outputBufferSize = JSON_OBJECT_SIZE(7);

DynamicJsonBuffer jsonBuffer(inputBufferSize);
StaticJsonBuffer<outputBufferSize> outputBuffer;

String actuate( byte* payload) {
   
  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
  }
  
  //  Extract values from parsed json object 
  temp        = root["temp"];
  humidity    = root["humi"];
  pm10        = root["PM10"] ;
  wind_speed  = root["WS"];
  dew_point   = root["DP"];

  /*  
   *   Calculate weighted values 
   */
  
  // FAN
  float fan_val = 3.5*humidity/50 + 3*dew_point/60 + 3*pm10/100 + 0.5*wind_speed/10;
  Serial.print("Fan Value :");
  Serial.println(fan_val);
  
  // LIGHT
  float light_val = 4*temp/20 + 4*water_temp/25 + humidity/50 + dew_point/60  ;
  Serial.print("Light Value :");
  Serial.println(light_val);

  /*
   * Actuator decision
   */
  int fan, light, water_valve;

  Serial.println("========== ACTUATIONS ==========");
  Serial.println();
  
 //FAN
  if (fan_val < 9){
    fan = 0;
    Serial.println ("Fan turned OFF");
    Serial.println();
  }else if(9<=fan_val<11){
    fan = 1;
    Serial.println ("Fan turned ON - Level 1");
    Serial.println();
  }else if(11<=fan_val<13){
    fan = 2;
    Serial.println ("Fan turned ON - Level 2");
    Serial.println();
  }else{
    fan = 3;
    Serial.println ("Fan turned ON - Level 3");
    Serial.println();
  };

  //LIGHT
  if (light_val > 13){
    light = 0;
    Serial.println ("UV light turned OFF ");
    Serial.println();
  }else if(11<= light_val<13){
    light = 1;
    Serial.println ("UV light turned ON - Level 1 ");
    Serial.println();
  }else if(9<=light_val<11){
    light = 2;
    Serial.println ("UV light turned ON - Level 2 ");
    Serial.println();
  }else{
    light = 3;
    Serial.println ("UV light turned ON - Level 3 ");
    Serial.println();
  };

  //WATER VALVE
  if (water_level ==3){
    water_valve = 0;
    Serial.println ("Water valve turned OFF ");
    Serial.println();
  }else if(water_level==2){
    water_valve = 1;
    Serial.println ("Water valve turned ON - Level 1");
    Serial.println();
  }else if(water_level==1){
    water_valve = 2;
    Serial.println ("Water valve turned ON - Level 2");
    Serial.println();
  }else if (water_level==0){
    water_valve = 3;
    Serial.println ("Water valve turned ON - Level 3");
    Serial.println();
  };
 
/*
 * Create output json object to be sent to node red
 */
  JsonObject& out     = outputBuffer.createObject();

  out["battery"]      = battery; 
  out["fan"]          = fan; 
  out["light"]        = light; 
  out["water_valve"]  = water_valve; 
  out["water_level"]  = water_level; 
  out["water_temp"]   = water_temp;
  out["ph_level"]     = ph_level;

  // Convert json to string
  out.printTo(outputToNodeRed);
  
  Serial.println();
  Serial.println(outputToNodeRed);
  
  return (outputToNodeRed);
};
