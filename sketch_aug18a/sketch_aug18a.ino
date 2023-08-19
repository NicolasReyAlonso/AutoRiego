#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "Servo.h"

LiquidCrystal_I2C LCD(0x27, 16, 2);
long a;
String b;
Servo servo;
int i;
const int buttonLeftPin = 2;
const int buttonRightPin = 3;
const int speakerPin = 9;
int buttonLeft;
int buttonRight;
int xorGate;

void setup() {
  Serial.begin(9600);
  digitalWrite(4, LOW);
  LCD.init();
  LCD.backlight();
  servo.attach(6);
  i = 0;
  buttonRight=0;
  buttonLeft=0;
}

void loop() {
  /*
    Más información será añadida a medida que amuente el código.

  */
  LCD.setCursor(1, 0);
  LCD.print("Water Level:");
  LCD.display();
  digitalWrite(4, HIGH);   
  a = analogRead(A0);
  digitalWrite(4, LOW);
  LCD.setCursor(1, 1);
  b = String(a);
  LCD.print(b);
  buttonLeft = digitalRead(buttonLeftPin);
  buttonRight = digitalRead(buttonRightPin);
  xorGate = buttonLeft^buttonRight;
  if (xorGate == HIGH){
    if (buttonLeft == HIGH){
      left();
    }else{
      right();
    }
  }
}

void right(){
   if (servo.read() == 10){
     tone(speakerPin, 300, 200);
     delay(100);
     tone(speakerPin, 200, 200);
   }else{
     tone(speakerPin, 1000, 200);
   }
   servo.write(10);
   delay(500);
}
void left(){
   if (servo.read() == 160){
     tone(speakerPin, 300, 200);
     delay(100);
     tone(speakerPin, 200, 200);
   }else{
     tone(speakerPin, 500, 200);
   }
   servo.write(160);
   delay(500);

}