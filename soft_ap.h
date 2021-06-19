#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

/*access point credentials*/
const char* ap_ssid = "TEST_AP";
const char* ap_password = "12345678";

String wifi_credentials[2];
String wifi_pw;
String wifi_ssid;

AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";


// HTML web page to handle 3 input fields (input1, input2)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Input WiFi Credentials</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h2 style="color: #097479;text-align: center"> HydroPhonic Farming Solution</h2>
      <p>Enter Your WiFi Credentials </p>
      <form action="/get">
        SSID: <input type="text" name="input1">
        <br><br>
        PASSWORD: <input type="text" name="input2">
        <br>
        <input type="submit" value="Submit" style="color: white; background-color:#097479; padding:10px 40px;margin: 20px 10px">
      </form><br>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void start_softAP(){
    Serial.println("########## STARTING SOFT AP");
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println();
    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP());
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", index_html);
    });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {       
        // GET input values from URL parameters
        if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
        wifi_pw = request->getParam(PARAM_INPUT_2)->value();
        wifi_ssid = request->getParam(PARAM_INPUT_1)->value();
        request->send(200, "text/html", "Credentials submitted: <br><a href=\"/\">Return to Home Page</a>");
        }
        else {
        request->send(200, "text/html", "Please re-enter the credentials: <br><a href=\"/\">Return to Home Page</a>");     
        }
        
        Serial.println(wifi_pw);
        Serial.println(wifi_ssid);  
    });
    server.onNotFound(notFound);
    server.begin();
}

void reset_credentials(){
  wifi_pw = "";
  wifi_ssid = "";
}

String get_ssid(){
  //Serial.println("########## GET SSID");
  return wifi_ssid;
}

String get_password(){
  //Serial.println("########## GET PASSWORD");
  return wifi_pw;
}
