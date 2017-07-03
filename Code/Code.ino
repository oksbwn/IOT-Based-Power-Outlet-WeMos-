#include <ESP8266WiFi.h>

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
const char* ssid = "weargenius"; // SSID Of the Router
const char* password = "omisoksbwn";// Access point Password
const char* host = "http://api.thingspeak.com"; //Server IP or URL https://api.thingspeak.com/channels/297083/feeds
String url = "/channels/297083/feeds"; //Page path
int requestTime=-120000;
int exitT=1;
//Static IP Setup
IPAddress ip(192, 168, 0, 5); // this 3 lines for a fix IP-address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// the setup function runs once when you press reset or power the board
void setup() {
  initialize();
  allOff();
}

// the loop function runs over and over again forever
void loop() {
  if(millis()-requestTime>120000){ // Request data from the Srever after every 2 Minutes
    requestTime=millis();
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
    if (millis() - requestTime >30000) { // 30 seconds to wait for response before timeout
      Serial.println(">>> Client Timeout !");
      client.stop();
      exitT=0;
      break;
    }
    }
    String line="";
    while(client.available()){ // Read response from Server
      line += client.readStringUntil('\r');
    }
    line=line.substring(line.indexOf('[')+1,line.indexOf(']')); // Process the response
    Serial.println(line);
 

    if(exitT==1){ 
      exitT=1;
      Serial.println("closing connection");
      client.stop();
    }
  }
}

//Initialize Routine
void initialize(){
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
  WiFi.config(ip, gateway, gateway, subnet);
  WiFi.begin(ssid, password); // Initiate connection to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //Printout the IP assigned to the module in DHCP
  
  }
//Make Everything OFF

void allOff(){
    
  digitalWrite(SOCKET_ONE, LOW);
  digitalWrite(SOCKET_ONE_LED, LOW);
  digitalWrite(SOCKET_TWO, LOW);
  digitalWrite(SOCKET_TWO_LED, LOW);
  digitalWrite(SOCKET_THREE, LOW);
  digitalWrite(SOCKET_THREE_LED, LOW);
  digitalWrite(SOCKET_FOUR, LOW);
  digitalWrite(SOCKET_FOUR_LED, LOW);
  }


