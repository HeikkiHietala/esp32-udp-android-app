#include "WiFi.h"
#include "AsyncUDP.h"
#include "heltec.h"
#include "images.h"

const char* ssid = "LMHOP";
const char* pass = "Ossi1Paavo234";
String udpSpeed, udpDir;
int localPort = 1234;
int driveSpeed, driveDir;

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

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
  uint8_t color = 1;
  for (int16_t i = 0; i < DISPLAY_HEIGHT / 2; i += 3) {
    Heltec.display->setColor((color % 2 == 0) ? BLACK : WHITE); // alternate colors
    Heltec.display->fillRect(i, i, DISPLAY_WIDTH - i * 2, DISPLAY_HEIGHT - i * 2);
    Heltec.display->display();
    delay(10);
    color++;
  }
  // Reset back to WHITE
  Heltec.display->setColor(WHITE);
  Heltec.display -> clear();
                                                                                                                                                   
  Heltec.display -> drawXbm(0, 0, hhlogo_width, hhlogo_height, hhlogo_bits);
  Heltec.display -> display();
  delay(2000);
  Heltec.display -> clear();
  Heltec.display -> drawXbm(0, 0, robolablogo_width, robolablogo_height, robolablogo_bits);
  Heltec.display -> display();
  delay(2000);
  Heltec.display -> clear();
  Heltec.display->setFont(ArialMT_Plain_10);
  // Initialize Serial Monitor
  Heltec.display->drawString(0, 0, "Connecting to WLAN");
  Heltec.display -> display();
  Serial.begin(115200);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "WiFi connected.");
  Heltec.display->drawString(0, 12, "IP address: ");
  Heltec.display->drawString(0, 24, String(WiFi.localIP().toString()));
  Heltec.display->drawString(0, 36, "UDP Listening on: " + String(localPort));

  Heltec.display -> display();
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

  Heltec.display->setFont(ArialMT_Plain_10);

  if (udp.listen(localPort)) {
    //Heltec.display->clear();
    delay(100);
    udp.onPacket([](AsyncUDPPacket packet) {
      char buf[packet.length() + 1] = {};
      memcpy(buf, packet.data(), packet.length());
      String s = String(buf);
      int udpSplit = s.indexOf(",");
      udpSpeed = s.substring(0, udpSplit);
      udpDir = s.substring(udpSplit + 1, packet.length());
      Heltec.display->clear();
      Heltec.display->drawString(0, 0, "Incoming packet: " + s);
      Heltec.display->setFont(ArialMT_Plain_16);
      Heltec.display->drawString(0, 15, "Speed: " + udpSpeed);
      Heltec.display->drawString(0, 35, "Direction: " + udpDir);
      Heltec.display -> display();
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
