/*
Arduino Weather Station + MeteorJS
Autor: Pedro Valverde
 */
#include <Wire.h>
#include <BMP085.h>
#include <DHT.h>
#include <SPI.h>
#include <Ethernet.h>

DHT dht;
int led = 7;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "ws1.meteor.com";

//Manual Setup
byte ip[] = { 10, 30, 8, 43 };   //Manual setup only
byte gateway[] = { 192, 168, 0, 1 }; //Manual setup only
byte subnet[] = { 10, 30, 8, 1 }; //Manual setup only

// Initialize the Ethernet client library
EthernetClient client;

void setup(){
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial.println();

  dht.setup(2); // data pin 2 (DHT22)
  bmp085Calibration();
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Manual Setup
    Ethernet.begin(mac, ip, gateway, subnet);
  }
  // give the Ethernet shield a second to initialize:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

}

void loop()
{
  
  delay(dht.getMinimumSamplingPeriod());


  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  float temperature2 = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; // "standard atmosphere"

//Construct the query
  char temp[10];
  String tempAsString;
  String req = "GET /log/";
  
  dtostrf(temperature,1,2,temp);
  req = req + String(temp) + "/";
  dtostrf(humidity,1,2,temp);
  req = req + String(temp) + "/";
  dtostrf(atm,1,4,temp);
  req = req + String(temp);
  req = req +" HTTP/1.1";
  Serial.println(req);

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("sending...");
        
    // Make a HTTP request:
    client.println(req);
    client.println("Host: ws1.meteor.com");
    //client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    Serial.println("sended");
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
    //client.stop();
  }
  
// Stotus LED  
  digitalWrite(led,HIGH);
  delay(100);
  digitalWrite(led,LOW);

  delay(600000); //wait a 10 minutes (10x60x1000) and get values again.
  client.stop();
}

