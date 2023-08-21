#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "Servo.h"

/*
  Values of LiquidCrystal_I2C:
  I2C adress
  Number of Digits
  Number of Lines
*/
LiquidCrystal_I2C LCD(0x27, 16, 2);
long a;
String b;
Servo servo;
int i;
const int buttonLeftPin = 2;
const int buttonRightPin = 3;
const int speakerPin = 9;
const int fotodiodoPin = A3;
int fotodiodoValue;
bool xorGate;
bool flagButtonLeft;
bool flagButtonRight;

/*
  Setup of the pins, the modes and the methos responsible for handling interruptions
*/
void setInterruptions(){
  pinMode(buttonLeftPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonLeftPin), leftButtonFlag, RISING);
  pinMode(buttonRightPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonRightPin), rightButtonFlag, RISING);
}

/*
  Initializer of values
*/
void initializeVariables(){
  i = 0;
  flagButtonLeft = false;
  flagButtonRight = false;
}

void setup() {
  setInterruptions();
  initializeVariables();
  Serial.begin(9600);
  LCD.init();
  LCD.backlight();
  servo.attach(6);
  
}

void loop() {
  /*
    Light levels are checked and if they are too high the system asks for manual confirmation to operate
  */
  fotodiodoValue = analogRead(fotodiodoPin);
  if (fotodiodoValue > 300){
    LCD.setCursor(0, 0);
    LCD.print("LightLevel too");
    LCD.setCursor(0, 1);
    LCD.print("high, proceed?");
    LCD.display();
    if (flagButtonLeft){
      flagButtonLeft = false;
      Start();

    }
  }else{
    Start();
  }
}

/*
  Plant watering protocol
*/
void Start() {
  LCD.clear();
  while (true) {
    LCD.setCursor(1, 0);
    LCD.print("Water Level:");
    LCD.display();
    a = analogRead(A0);
    LCD.setCursor(1, 1);
    b = String(a);
    LCD.print(b);
    //Checks that both buttons are not pressed at the same time
    xorGate = flagButtonLeft ^ flagButtonRight;
    if (xorGate) {
      if (flagButtonLeft) {
        flagButtonLeft = false;
        flagButtonRight = false;
        Close();
      } else {
        flagButtonLeft = false;
        flagButtonRight = false;
        Open();
      }
    }
  }
}

/*
  Instructions performed during an interruption triggered by the LeftButton
*/
void leftButtonFlag(){
  flagButtonLeft = true;
}
/*
  Instructions performed during an interruption triggered by the RightButton
*/
void rightButtonFlag(){
  flagButtonRight = true;
}

/*
  Methot that opens the valve to control the flow of water
*/
void Open() {
  if (servo.read() == 10) {
    tone(speakerPin, 300, 200);
    delay(100);
    tone(speakerPin, 200, 200);
  } else {
    tone(speakerPin, 1000, 200);
  }
  servo.write(10);
  delay(500);
}
/*
  Methot that closes the valve to control the flow of water
*/
void Close() {
  if (servo.read() == 160) {
    tone(speakerPin, 300, 200);
    delay(100);
    tone(speakerPin, 200, 200);
  } else {
    tone(speakerPin, 500, 200);
  }
  servo.write(160);
  delay(500);
}