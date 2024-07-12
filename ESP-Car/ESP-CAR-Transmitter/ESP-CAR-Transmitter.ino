// Transmitter
#include <WiFiUdp.h>
#include <WiFi.h>
WiFiUDP udp;


const char* ssid = "the ssid";  //im not putting my details there lol
const char* password = "the password";
const int speedMode = 27;
const int speedLed = 14;
const int lS = 40;
const int steering = 32; // 35 for joystick, 32 for external
const int throttle = 34;
int throttleValue;
int steerValue;
int maxSpeed;
unsigned int localUdpPort = 1234;
IPAddress receiverIP(172,20,10,3); 


void setup() {
  Serial.begin(115200);
 pinMode(steering, INPUT);
 pinMode(throttle, INPUT);
 pinMode(speedMode, INPUT_PULLUP);
 pinMode(speedLed, OUTPUT);
 maxSpeed = lS;


 // Connect to Wi-Fi
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");


 Serial.print("ESP32 IP Address: ");
 Serial.println(WiFi.localIP());
}


void loop() {
int buttonState = digitalRead(speedMode);


 if (buttonState == LOW) {
   delay(50);
   if (maxSpeed == lS) {
     maxSpeed = 180;
   } else {
     maxSpeed = lS;
   }
   digitalWrite(speedLed, maxSpeed == 180 ? HIGH : LOW);
   while (digitalRead(speedMode) == LOW) {
     delay(10);
   }
 }
 throttleValue = analogRead(throttle);
 steerValue = analogRead(steering);


 throttleValue = constrain(throttleValue, 0, 1800);
 int speed = map(throttleValue, 1800, 0, 0, maxSpeed);
 steerValue = constrain(steerValue, 1900, 2800);
 int angle = map(steerValue, 1900, 2800, 0, 160);//1000,4000


 // Send the potentiometer value to the ESP8266
 udp.beginPacket(receiverIP, localUdpPort);
 udp.write((uint8_t*)&speed, sizeof(speed));
  udp.write((uint8_t*)&angle, sizeof(angle));
 udp.endPacket();
 Serial.println(steerValue);


 delay(50);
}
