#include <Servo.h> 
#include <IRremote.hpp> 

const int IR_RECEIVE_PIN = 2; // The IR Sensor is connected to Pin 2 
const int rDriverPin = 6; // The right servo motor is connected to Pin ~6 
const int lDriverPin = 5; // The right servo motor is connected to Pin ~5 
int driveSpeed = 90; // Tell the microcontroller that the terms "rightDriver" and "leftDriver" are referring to the servo motors 
Servo rightDriver; 
Servo leftDriver; 

// Ultrasonic Sensor stuff 
const byte triggerPin = 3; 
const byte echoPin = 4; 

// IR Button Definitions (to add your own custom remote buttons, see customRemote.ino or visit WAMdocs.github.io/IR/customremote) 
#define power 0x1E 
#define eject 0x5D 

#define t1 0x53 
#define t2 0x19 
#define t3 0x52 
#define t4 0x5A 

#define one 0x1A 
#define two 0x1F 
#define three 0x58 
#define four 0x16 
#define five 0x1B 
#define six 0x54 
#define seven 0x12 
#define eight 0x17 
#define nine 0x50 
#define zero 0x13 

#define dSdd 0xE 
#define repeat 0x4C 

#define volUp 0xA 
#define volDn 0x6 
#define chUp 0x48 
#define chDn 0x44 

#define mute 0x5C 
#define info 0xC 

#define title 0x18 
#define epg 0x4D 
#define menu 0x1C 
#define source 0x7 

#define up 0x3 
#define down 0x5E 
#define left 0x2 
#define right 0x40 
#define ok 0x5F 

#define red 0x55 
#define green 0x56 
#define yellow 0x57 
#define blue 0x14 

#define b1 0x59 
#define b2 0x10 
#define b3 0x42 
#define b4 0x15 
#define b5 0x5b 
#define b6 0x4E 
#define b7 0x4F 
#define b8 0x60 
#define b9 0x11 
#define b10 0x49 
#define b11 0x4A 

uint8_t command = 0; // This is where the last pressed button is stored 
int mode = 1; // This gets changed if you press any numbered button (0-9)
int IRDEBUG = 1; // Change to 1 if you want the IR codes printed in serial as they are received (a.k.a IR debug mode) 

void setup() { 
  Serial.begin(9600); 
  Serial.println("--------------START PROGRAM--------------");
  rightDriver.attach(rDriverPin); // Tell the microcontroller what pin the right motor is plugged into (defined at top) 
  leftDriver.attach(lDriverPin); // Tell the microcontroller what pin the left motor is plugged into (defined at top) 
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Tell the microcontroller what pin the IR receiver is plugged into (defined at top) 
} 

void loop() { 
  if (IrReceiver.decode()) { // If the IR Receiver detects a signal 
    // If not in IR debug mode (defined at top), then the serial will print: (e.g.) "The up button was pressed". If not, then: (e.g.) "up command (0x3) was pressed" 
    if (IRDEBUG == 0) Henry_v2_DEBUG_inline1: if (IRDEBUG == 0) Serial.print("The "); 

    command = IrReceiver.decodedIRData.command; 
    switch (command) { // Check if any numbered buttons are pressed 
      case one: 
        mode = 1; 
        Serial.print("mode 1 "); 
        break; 
      case two: 
        mode = 2; 
        Serial.print("mode 2 "); 
        break; 
      case three: 
        mode = 3; 
        Serial.print("mode 3 "); 
        break; 
      case four: 
        mode = 4; 
        Serial.print("mode 4 "); 
        break; 
      case five: 
        mode = 5; 
        Serial.print("mode 5 "); 
        break; 
      case six: 
        mode = 6; 
        Serial.print("mode 6 "); 
        break; 
      case seven: 
        mode = 7; 
        Serial.print("mode 7 "); 
        break; 
      case eight: 
        mode = 8; 
        Serial.print("mode 8 "); 
        break; 
      case nine: 
        mode = 9; 
        Serial.print("mode 9 "); 
        break; 
      case zero: 
        mode = 0; 
        Serial.print("mode 0 "); 
        break; 
      case t1: 
        if (IRDEBUG == 1) IRDEBUG = 0; else IRDEBUG = 1; 
        break; 
      default: 
        if (mode == 1) { // If the "1" button is pressed OR if no numbered button is pressed after starting 
          switch (command) { // Run through all buttons needed in mode 1 
            case ok: 
              stopDrivers(1); // If you want to print "STOP " in the serial, then add a 1 in the brackets
              break; 
            case up: 
              moveUp(); 
              break; 
            case down: 
              moveDown(); 
              break; 
            case left: 
              turnLeft(); 
              break; 
            case right: 
              turnRight(); 
              break; 
            case title: 
              spinLeft(); 
              break; 
            case epg: 
              spinRight(); 
              break; 
            case menu: 
              reverseLeft(); 
              break; 
            case source: 
              reverseRight(); 
              break; 
            default: 
              Serial.print("dis-used "); 
              break; 
          } 
        } 
        break; 
    } 
    // For debugging 
    if (IRDEBUG == 1) { 
      Serial.print("command (0x"); 
      Serial.print(command, HEX); 
      Serial.println(") received"); 
    } else {
      Serial.println("button was pressed");
    } 
    // !!! IMPORTANT! If not included, IR signals after this will be ignored. !!! 
    IrReceiver.resume(); 
    delay(10); 
  } 
} 

// Functions 
// When turning/spinning, turn/spin 45 degrees 
void stopDrivers(int fromCase) { 
  if (fromCase == 1) {
    Serial.print("STOP "); 
  }
  rightDriver.write(90); // Stop the right motor 
  leftDriver.write(90); // Stop the left motor 
  delay(500);
} 

void moveUp() { 
  Serial.print("up "); 
  rightDriver.write(90 - driveSpeed); 
  leftDriver.write(90 + driveSpeed); 
  delay(500); 
  stopDrivers(0); 
} 

void moveDown() { 
  Serial.print("down "); 
  rightDriver.write(90 + driveSpeed); 
  leftDriver.write(90 - driveSpeed); 
  delay(500); 
  stopDrivers(0); 
} 

void turnLeft() { 
  Serial.print("left "); 
  rightDriver.write(90 - driveSpeed); 
  leftDriver.write(90); 
  delay(500); 
  stopDrivers(0); 
} 

void turnRight() { 
  Serial.print("right "); 
  rightDriver.write(90); 
  leftDriver.write(90 + driveSpeed); 
  delay(500); 
  stopDrivers(0); 
} 

void reverseLeft() { 
  Serial.print("reverse left "); 
  rightDriver.write(90 - driveSpeed); 
  leftDriver.write(90); 
  delay(500); 
  stopDrivers(0); 
} 

void reverseRight() { 
  Serial.print("reverse right "); 
  rightDriver.write(90); 
  leftDriver.write(90 + driveSpeed); 
  delay(500); 
  stopDrivers(0); 
} 

void spinLeft() { 
  Serial.print("spin left "); 
  rightDriver.write(90 - driveSpeed); 
  leftDriver.write(90 - driveSpeed); 
  delay(500); 
  stopDrivers(0); 
} 

void spinRight() { 
  Serial.print("spin right "); 
  rightDriver.write(90 + driveSpeed); 
  leftDriver.write(90 + driveSpeed); 
  delay(500); 
  stopDrivers(0); 
}
