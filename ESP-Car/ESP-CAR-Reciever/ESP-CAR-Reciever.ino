// Reciever
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


#include <Servo.h>
Servo driveM;
Servo steerM;


const char* ssid = "the ssid";  //im not putting my details there lol
const char* password = "the password";
unsigned int localUdpPort = 1234;


WiFiUDP udp;

int speed;
int angle;

void setup() {
 Serial.begin(115200);


 // Connect to Wi-Fi
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");


 Serial.print("ESP8266 IP Address: ");
 Serial.println(WiFi.localIP());


 // Start UDP comms on the local UdpPort
 udp.begin(localUdpPort);


driveM.attach(12, 1000, 2000);
 steerM.attach(4);


 // Initialize the servos
 driveM.write(0); 
 steerM.write(45);


}


void loop() {
while (WiFi.status() == WL_CONNECTED) { 
 int packetSize = udp.parsePacket();
 
   udp.read((uint8_t*)&speed, sizeof(speed));
  
   driveM.write(speed);
   Serial.println(speed);
   udp.read((uint8_t*)&angle, sizeof(angle));
   steerM.write(angle);
}
driveM.write(0); 

}
