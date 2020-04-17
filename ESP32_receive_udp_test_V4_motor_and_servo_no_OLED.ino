#include "WiFi.h"
#include "AsyncUDP.h"

const char* ssid = "LMHOP";
const char* pass = "Ossi1Paavo234";
String udpSpeed, udpDir;
int driveSpeed, driveDir;
int localPort = 1234;

AsyncUDP udp;

// Motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;
// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;
// Servo
const int turnPin = 12;  //Steering Servo Pin
int myTurn;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("Connecting to WLAN");

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(String(WiFi.localIP().toString()));
  Serial.println("UDP Listening on: " + String(localPort));
  delay(2000);
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);

  ledcSetup(4, 50, 16); //channel, freq, resolution for servo
  ledcAttachPin(turnPin, 4); // pin, channel
}

void loop()
{
  if (udp.listen(localPort)) {
    delay(100);
    udp.onPacket([](AsyncUDPPacket packet) {
      char buf[packet.length() + 1] = {};
      memcpy(buf, packet.data(), packet.length());
      String s = String(buf);
      int udpSplit = s.indexOf(",");
      udpSpeed = s.substring(0, udpSplit);
      udpDir = s.substring(udpSplit + 1, packet.length());
      Serial.println("Incoming packet: " + s);
      Serial.println("Speed: " + udpSpeed);
      Serial.println("Direction: " + udpDir);
      driveSpeed = udpSpeed.toInt();
      driveDir = udpDir.toInt();
      Serial.println(driveSpeed);
      runMotor (driveDir, driveSpeed);
      delay(100);
    });
  }
}

void steeringAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 180) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}

void runMotor(int myDir, int mySpeed) {
  Serial.println(mySpeed);
  int finalSpeed;
  finalSpeed = map(abs(mySpeed), 0, 10, 215, 255);
  Serial.println(finalSpeed);
  Serial.println("Moving Forward");
  if (mySpeed > 0) {
    Serial.println("Forward");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
  } else if (mySpeed < 0) {
    Serial.println("Back");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
  } else {
    Serial.println("STOP");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
  }
  ledcWrite(pwmChannel, finalSpeed);
  myTurn =   map (myDir, -10, 11, 36, 180);
  steeringAnalogWrite(4, myTurn); // set steering
}
