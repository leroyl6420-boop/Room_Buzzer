// Hi! Welcome to the Room Buzzer code! Really, it's gonna be super
// simple, but I'm doing this mostly for the heartbeat. 

#include <WiFi.h>
#include <NetworkUdp.h>
NetworkUDP udp;
const char* laptopIP = "192.168.1.250";
const int laptopPort = 4210;
const char* wifiName = "Delepres";
const char* wifiPassword = "99993568";
#define RED_LED 25
#define BLUE_LED 26
#define RED_BUTTON 32
#define BLUE_BUTTON 33
#define BUZZER 27
bool BlueButtonPressed = false;
bool RedButtonPressed = false;
bool LastBlueButton = false;
bool LastRedButton = false;
unsigned long NotificationCooldown = 0;

void sendMessage(const char* message) {
    udp.beginPacket(laptopIP, laptopPort);
    udp.print(message);
    udp.endPacket();
    Serial.print("Sent: ");
    Serial.println(message);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Connecting to Wifi");
    WiFi.begin(wifiName, wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected!");
    Serial.print("ESP32 IP adress:");
    Serial.println(WiFi.localIP());
    udp.begin(4211);
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(BLUE_BUTTON, INPUT_PULLUP);
}

void loop() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    noTone(BUZZER);
    BlueButtonPressed = false;
    RedButtonPressed = false;
    if (digitalRead(BLUE_BUTTON) == LOW) {
        digitalWrite(BLUE_LED, HIGH);
        BlueButtonPressed = true;
    }
    if (digitalRead(RED_BUTTON) == LOW) {
        digitalWrite(RED_LED, HIGH);
        tone(BUZZER, 3000);
        RedButtonPressed = true;
    }
    if (BlueButtonPressed == true && LastBlueButton == false && millis() > NotificationCooldown){
        sendMessage("Casual Notification from Room Buzzer");
        NotificationCooldown = millis() + 1000;
    }
    if (RedButtonPressed == true && LastRedButton == false && millis() > NotificationCooldown){
        sendMessage("Urgent Notification from Room Buzzer");
        NotificationCooldown = millis() + 1000;
    }
    LastBlueButton = BlueButtonPressed;
    LastRedButton = RedButtonPressed;
}
