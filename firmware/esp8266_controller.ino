#define REMOTEXY_MODE__WIFI_POINT   // must come BEFORE including RemoteXY.h

#include <ESP8266WiFi.h>
#include <RemoteXY.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFiUdp.h>

WiFiUDP udp;
String laptopIP;
const unsigned int laptopPort = 5005;

#define REMOTEXY_WIFI_SSID "RC_Car_Dash"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 62 bytes
  { 255,4,0,1,0,55,0,19,0,0,0,0,31,1,200,84,1,1,5,0,
  4,121,48,75,12,176,24,26,4,25,31,11,45,16,2,26,1,37,19,16,
  16,0,2,31,0,1,6,20,16,16,0,2,31,0,67,81,34,19,29,94,
  2,26 };
  
struct {
  int8_t steering; 
  int8_t throttle; 
  uint8_t gearup; 
  uint8_t geardown; 
  int8_t value_01; 
  uint8_t connect_flag;  
} RemoteXY;   
#pragma pack(pop)

// ----- Pin Setup -----
const int forwardPWM = 5;    //d4
const int relayBackward = 2;    //d1

const int SERVO_PIN = 16; 

#define SS_PIN 15     
#define RST_PIN 4     

Servo steeringServo;
MFRC522 rfid(SS_PIN, RST_PIN);

// --- Updated Gears: Reverse (0) + Forward (1–6) ---
const float gearSpeeds[] = {30, 10, 20, 30, 45, 60, 80};  
// index 0 = Reverse (30 km/h limit)

int currentGear = 1;  // start in gear 1
float currentSpeed = 0;
unsigned long lastUpdate = 0;

bool zoneRestricted = false;
float restrictedSpeed = 20;
String zoneName = "Normal";
String currentTag = "";

void setup() {
  Serial.begin(115200);
  RemoteXY_Init();
  laptopIP ="192.168.4.2";

  pinMode(forwardPWM, OUTPUT);
  pinMode(relayBackward, OUTPUT);
  digitalWrite(relayBackward, HIGH);



  steeringServo.attach(SERVO_PIN);
  steeringServo.write(90);

  SPI.begin();
  rfid.PCD_Init();
  delay(100);

  Serial.println("\nRC Car Ready (ESP8266 + L298N + Servo + RFID + RemoteXY)");
}

void loop() {
  RemoteXY_Handler();
  unsigned long now = millis();

  // --- RFID Zone Detection ---
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String tagID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      tagID += String(rfid.uid.uidByte[i], HEX);
    }
    tagID.toUpperCase();
    currentTag = tagID;

    Serial.print("RFID Tag Detected: ");
    Serial.println(tagID);

    if (tagID == "53E8FF3") {  // Hospital IN
      zoneRestricted = true;
      zoneName = "Hospital Zone";
      Serial.println(">>> HOSPITAL ZONE DETECTED – SPEED LIMIT ENFORCED <<<");
      if (currentGear > 2) currentGear = 2;
    } 
    else if (tagID == "232F9FE4") {  // Hospital OUT
      zoneRestricted = false;
      zoneName = "Normal";
      Serial.println("<<< EXITED HOSPITAL ZONE – NORMAL MODE RESTORED >>>");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  // --- Control Logic ---
  if (now - lastUpdate > 100) {
    lastUpdate = now;

    static bool lastUp = 0, lastDown = 0;

    // --- Gear shifting ---
    if (RemoteXY.gearup && !lastUp) currentGear = min(currentGear + 1, 6);
    if (RemoteXY.geardown && !lastDown) currentGear = max(currentGear - 1, 0);
    lastUp = RemoteXY.gearup;
    lastDown = RemoteXY.geardown;

    // Restrict forward gears in special zones (but allow reverse)
    if (zoneRestricted && currentGear > 2) currentGear = 2;

    // --- Speed Control ---
    float acceleration = RemoteXY.throttle;
    float maxGearSpeed = gearSpeeds[currentGear];

    // Limit speed if zone restricted
    if (zoneRestricted && currentGear > 0) 
      maxGearSpeed = min(maxGearSpeed, restrictedSpeed);

    float targetSpeed = (acceleration / 100.0) * maxGearSpeed;
    if (currentSpeed < targetSpeed) currentSpeed += 2;
    else if (currentSpeed > targetSpeed) currentSpeed -= 3;
    currentSpeed = constrain(currentSpeed, 0, maxGearSpeed);

    int pwmValue = map(currentSpeed, 0, 80, 0, 1023);

    // --- Steering ---
    int servoPos = map(RemoteXY.steering, -99, 99, 0, 180);
    steeringServo.write(abs(servoPos-180));

    RemoteXY.value_01 = (currentGear == 0) ? 0 : currentGear;

    // --- Debug Info ---
    Serial.print("Zone: "); Serial.print(zoneName);
    Serial.print(" | Gear: ");
    if (currentGear == 0){ 
      digitalWrite(relayBackward,LOW);
      Serial.print("R");
    }
    else
    {
      Serial.print(currentGear);
      analogWrite(forwardPWM, pwmValue);
      digitalWrite(relayBackward,HIGH);
    } 
    Serial.print(" | Speed: "); Serial.print(currentSpeed, 1);
    Serial.print(" km/h | PWM: "); Serial.print(pwmValue);
    Serial.print(" | Steering: "); Serial.println(servoPos);

    // --- Send UDP Packet to Laptop ---
    if (laptopIP.length() > 0) {
      String msg = "Zone: " + zoneName + 
                   " | Tag: " + currentTag +
                   " | Gear: " + (currentGear == 0 ? String("R") : String(currentGear)) +
                   " | Speed: " + String(currentSpeed, 1) + " km/h";
      udp.beginPacket(laptopIP.c_str(), laptopPort);
      udp.print(msg);
      udp.endPacket();
    }
  }
}
