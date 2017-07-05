#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//Pin definations for connected sockets and corresponding LED
#define SOCKET_ONE D1
#define SOCKET_ONE_LED D5
#define SOCKET_TWO D4
#define SOCKET_TWO_LED D0
#define SOCKET_THREE D3
#define SOCKET_THREE_LED D6
#define SOCKET_FOUR D2
#define SOCKET_FOUR_LED D7

//Variables
const char* ssid = "YOUR_SSID"; // SSID Of the Router
const char* password = "************";// Access point Password
const char* host = "api.thingspeak.com"; //Server IP or URL https://api.thingspeak.com/channels/CHANNEL_ID/feeds?api_key=READ_API_KEY&results=1
String url = "/channels/CHANNEL_ID/feeds?api_key=READ_API_KEY&results=1"; //Page path
int requestTime = -120000;
int exitT = 1;
String json_data;
//Static IP Setup
IPAddress ip(192, 168, 0, 6); // this 3 lines for a fix IP-address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

//JSON Related
DynamicJsonBuffer jsonBuffer;
// the setup function runs once when you press reset or power the board
void setup() {
  initialize();
  allOff();
}

// the loop function runs over and over again forever
void loop() {
  if (millis() - requestTime > 30000) { // Request data from the Srever after every 0.5 Minutes
    requestTime = millis();
    Serial.print("connecting to ");
    Serial.println(host);

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) { //Connect to SERVER
      Serial.println("connection failed");
    }

    //Path of the webpage in the server to request
    Serial.print("Requesting URL: ");
    Serial.println(url);
    client.print(String("GET ") + url + " HTTP/1.0\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    while (client.available() == 0) { // check Response from server
      if (millis() - requestTime > 30000) { // 30 seconds to wait for response before timeout
        Serial.println(">>> Client Timeout !");
        client.stop();
        exitT = 0;
        break;
      }
    }
    String line = "";
    while (client.available()) { // Read response from Server
      line += client.readStringUntil('\r');
    }
    json_data = line.substring(line.indexOf('{'), line.lastIndexOf('}')+1); // Process the response
   
    Serial.println(json_data);
    JsonObject& root = jsonBuffer.parseObject(json_data);
    if (!root.success()) {
      Serial.println("Error in received JSOn File.");
      //return;
    }
    JsonArray& etc = root["feeds"];
    JsonObject& sensor = etc[0];
	const char* data=sensor["field1"];
	//Serial.println(data);
    changeStatus(1,sensor["field1"].as<int>());
    changeStatus(2,sensor["field2"].as<int>());
    changeStatus(3,sensor["field3"].as<int>());
    changeStatus(4,sensor["field4"].as<int>());

    if (exitT == 1) {
      exitT = 1;
      Serial.println("closing connection");
      client.stop();
    }
  }
}

//Initialize Routine
void initialize() {
  Serial.begin(115200);
  //All pins as output
  pinMode(SOCKET_ONE, OUTPUT);
  pinMode(SOCKET_ONE_LED, OUTPUT);
  pinMode(SOCKET_TWO, OUTPUT);
  pinMode(SOCKET_TWO_LED, OUTPUT);
  pinMode(SOCKET_THREE, OUTPUT);
  pinMode(SOCKET_THREE_LED, OUTPUT);
  pinMode(SOCKET_FOUR, OUTPUT);
  pinMode(SOCKET_FOUR_LED, OUTPUT);
  //Wi-Fi Connection
  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //WiFi.config(ip, gateway, gateway, subnet);
  WiFi.begin(ssid, password); // Initiate connection to the Wi-Fi network
  // WiFi.begin(ssid); // Initiate connection to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //Printout the IP assigned to the module in DHCP

}
//Change Status of a LOAD
void changeStatus(int load,int newStatus){
    Serial.print("Changing LOAD Status LOAD: ");
    Serial.print(load);
    Serial.print("Status");
    Serial.println(newStatus);
    switch(load){
      case 1:
        digitalWrite(SOCKET_ONE, newStatus);
        digitalWrite(SOCKET_ONE_LED, newStatus);
        break;
      case 2:
        digitalWrite(SOCKET_TWO, newStatus);
        digitalWrite(SOCKET_TWO_LED, newStatus);
        break;
      case 3:
        digitalWrite(SOCKET_THREE, newStatus);
        digitalWrite(SOCKET_THREE_LED, newStatus);
        break;
      case 4:
        digitalWrite(SOCKET_FOUR, newStatus);
        digitalWrite(SOCKET_FOUR_LED, newStatus);
        break;
      }  
  
  }
//Make Everything OFF

void allOff() {
  digitalWrite(SOCKET_ONE, LOW);
  digitalWrite(SOCKET_ONE_LED, LOW);
  digitalWrite(SOCKET_TWO, LOW);
  digitalWrite(SOCKET_TWO_LED, LOW);
  digitalWrite(SOCKET_THREE, LOW);
  digitalWrite(SOCKET_THREE_LED, LOW);
  digitalWrite(SOCKET_FOUR, LOW);
  digitalWrite(SOCKET_FOUR_LED, LOW);
}
