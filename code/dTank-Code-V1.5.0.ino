#include <Servo.h> 
#include <IRremote.hpp> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HCSR04.h>


// This is for the small blue/yellow screen for a mouth
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C
#define WIRE Wire
const int mouthX = 64; 
const int mouthY = 20;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);


const int IR_RECEIVE_PIN = 2; // The IR Sensor is connected to Pin 2 
const int rDriverPin = 6; // The right servo motor is connected to Pin ~6 
const int lDriverPin = 5; // The left servo motor is connected to Pin ~5 
int driveSpeed = 90; 
Servo rightDriver; 
Servo leftDriver; 

// Ultrasonic Sensor stuff 
const byte triggerPin = 3; 
const byte echoPin = 4; 

// IR Button Definitions 
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
int IRDEBUG = 1; // Change to 1 if you want the IR codes printed in serial as they are received 

void setup() { 
  Serial.begin(9600); 
  Serial.println("--------------START PROGRAM--------------");
  
  rightDriver.attach(rDriverPin); 
  leftDriver.attach(lDriverPin); 
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;); 
  }
  display.clearDisplay();
  
  // Seed the random generator using an unconnected analog pin
  randomSeed(analogRead(0)); 

  // Start the distace sensor
  HCSR04.begin(triggerPin, echoPin);
} 

void loop() { 
  // 
  updateMouthExpressions();
  double* distances = HCSR04.measureDistanceCm();

  if (IrReceiver.decode()) { 
    if (IRDEBUG == 0) Serial.print("The "); 

    command = IrReceiver.decodedIRData.command; 
    switch (command) { 
      case one: 
        mode = 1; 
        Serial.print("mode 1 "); 
        break; 
      case two: 
        mode = 2; 
        Serial.print("mode 2 "); 
        delay(800);
        break; 
      case three: 
        mode = 3; 
        Serial.print("mode 3 "); 
        delay(800);
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
        if (mode == 1) { 
          switch (command) { 
            case ok: 
              stopDrivers(1); 
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
    
    if (IRDEBUG == 1) { 
      Serial.print("command (0x"); 
      Serial.print(command, HEX); 
      Serial.println(") received"); 
    } else {
      Serial.println("button was pressed");
    } 
    IrReceiver.resume(); 
  } 
  delay(50); // Small cycle delay
} 

// ==========================================
// DRIVER AND MOTOR FUNCTIONS
// ==========================================
void stopDrivers(int fromCase) { 
  if (fromCase == 1) {
    Serial.print("STOP "); 
  }
  rightDriver.write(90); 
  leftDriver.write(90); 
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

void updateMouthExpressions() {
  // Timing Engines
  static unsigned long lastFrameTime = 0;
  static unsigned long lastStateTime = 0;
  static unsigned long timelineMarker = 0;
  static unsigned long talkingDuration = 0;

  static bool showingInfo = false;
  
  // Size Mapping State Tracker
  static int currentWidth = 30;
  static int currentHeight = 8;
  static int targetWidth = 30;
  static int targetHeight = 8;
  
  // Flags
  static bool isTalkingMode = false;
  static int talkingPose = 0;
  static unsigned long nextModeChangeDelay = 4000; // Triggers first chat routine at 4s

  const int centerX = SCREEN_WIDTH / 2;
  const int centerY = SCREEN_HEIGHT / 4; // Exactly Y/4 (16 pixels from top edge)
  unsigned long currentMillis = millis();

  // 1. TIMELINE ENGINE: Manages resting smile vs talking windows
  if (currentMillis - timelineMarker >= nextModeChangeDelay) {
    timelineMarker = currentMillis;
    
    if (!isTalkingMode) {
      // Transition from Resting Smile -> Active Random Talking Sequence
      isTalkingMode = true;
      talkingDuration = random(500, 2000);        // Random talking sequence between 0.5 and 2 seconds
      nextModeChangeDelay = talkingDuration;       // Match countdown window to duration
    } else {
      // Transition from Talking Sequence -> Back to Fixed Smile
      isTalkingMode = false;
      nextModeChangeDelay = random(3000, 8000);   // Rest period between 3 and 8 seconds
      
      // Force return targets back to the smile's structural envelope
      targetWidth = 30;
      targetHeight = 8;
    }
  }

  // 2. TALKING POSE ENGINE: Changes mouth shapes continuously *only* during a talking frame
  if (isTalkingMode) {
    if (currentMillis - lastStateTime >= 150) {   // Steps speech postures every 150ms
      lastStateTime = currentMillis;
      talkingPose = random(0, 4);                 // Cycles: flat line, ah, oh, or shocked circle

      switch(talkingPose) {
        case 0: targetWidth = 24; targetHeight = 1;  break; // Neutral flat line
        case 1: targetWidth = 28; targetHeight = 6;  break; // Squished Wide ("Ah" / 4)
        case 2: targetWidth = 14; targetHeight = 10; break; // Tall Narrow ("Oh" / 4)
        case 3: targetWidth = 16; targetHeight = 16; break; // Shocked Circle shape
      }
    }
  }

  // 3. GRAPHICS INTERPOLATION ENGINE: Adjusts sizes dynamically every 20ms
  if (currentMillis - lastFrameTime >= 20) {
    lastFrameTime = currentMillis;

    // Linearly steps structural transformations
    if (currentWidth < targetWidth) currentWidth += 2; 
    else if (currentWidth > targetWidth) currentWidth -= 2;

    if (currentHeight < targetHeight) currentHeight += 1; 
    else if (currentHeight > targetHeight) currentHeight -= 1;

    display.clearDisplay();

    // 4. RENDERING ROUTINES
    if (!isTalkingMode) {
      // FIXED RESTING SMILE
      // Uses mask bit values 4 (bottom-right) and 8 (bottom-left) combined (4 | 8 = 12)
      // to draw one seamless, single-primitive curving bottom arch.
      int smileRadius = currentWidth / 2;
      if (!showingInfo) {
      display.drawCircleHelper(centerX, centerY - (currentHeight / 2), smileRadius, 12, SSD1306_WHITE);
      }
      
    } else if (talkingPose == 3) {
      // SHOCKED STATE
      // Renders an un-skewed geometric frame using a true uniform circle primitive
      int radius = currentHeight / 2;
      if (radius < 2) radius = 2; // Bottom safety constraint
      display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);
      
    } else {
      // NORMAL TALKING SPEECH CAPSULES
      if (currentHeight <= 1) {
        display.drawLine(centerX - (currentWidth / 2), centerY, centerX + (currentWidth / 2), centerY, SSD1306_WHITE);
      } else {
        display.fillRoundRect(centerX - (currentWidth / 2), centerY - (currentHeight / 2), currentWidth, currentHeight, currentHeight / 2, SSD1306_WHITE);
      }
    }

    display.display();
  }
}
