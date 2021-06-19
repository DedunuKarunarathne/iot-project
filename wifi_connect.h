#define max_attempts 10

int attempts = 0;

bool setup_wifi(String ssid, String password) {

  delay(100);
  Serial.println("=======================================");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(&ssid[0], &password[0]);

  while (WiFi.status() != WL_CONNECTED) {
    attempts += 1;
    delay(1000);
    Serial.print(".");
    if (attempts == max_attempts){
      Serial.println("Cannot connect to WiFi");
      Serial.println("Re-enter the Credentials");
      attempts = 0;
      return false;      
    }
  }

  randomSeed(micros());
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("=======================================");
  
  return true;
}
