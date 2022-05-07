#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "WIFI_EY_0"
#define STAPSK  "23852385"
#endif
#define ESP8266_GPIO4    4


const char* ssid     = STASSID;
const char* password = STAPSK;

IPAddress ip(192,168,0,250);     
IPAddress gateway(192,168,0,1);   
IPAddress subnet(255,255,255,0);   


ESP8266WebServer server(80);

const int led = LED_BUILTIN;

const String postForms = "yeah";

unsigned long previousMillis = 0;
unsigned long interval = 43200000;



void handleRoot() {
  
  server.send(200, "text/html", postForms);
  
}


void handleGetArguments(){
  String message = server.arg("value");
  if(message == "1"){
  server.send(200, "text/plain", message);
  digitalWrite( ESP8266_GPIO4, HIGH );
  Serial.println("encendido");
  digitalWrite(led, 1);
  }else{
    server.send(200, "text/plain", message);
    digitalWrite( ESP8266_GPIO4, LOW );
    Serial.println("apagado");
    digitalWrite(led, 0);
    }
  }

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode( ESP8266_GPIO4, OUTPUT );
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);


  server.on("/getform", handleGetArguments);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();

//print the Wi-Fi status every 30 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >=interval){
    switch (WiFi.status()){
      case WL_NO_SSID_AVAIL:
        Serial.println("Configured SSID cannot be reached");
        break;
      case WL_CONNECTED:
        Serial.println("Connection successfully established");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("Connection failed");
        ESP.restart();
        break;
    }
    Serial.printf("Connection status: %d\n", WiFi.status());
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
    previousMillis = currentMillis;
  }

  
}
