#include "Movement.h"

#include "Timer.h" // For the timer interrupts

int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;     //M1 Direction Control
int M2 = 7;     //M1 Direction Control

// assuming pwm movement 
const int Enable1 = 5;    //Motor1 Speed Control
const int Enable2 = 6;    //Motor2 Speed Control
const int Motor1 = 4;     //Motor1 Direction Control
const int Motor2 = 7;     //Motor1 Direction Control
const byte LRE = 6;       //Left rotary encoder pin holder value
const byte RRE = 5;       //Right rotary encoder pin holder value
volatile byte LRC =0;     //Left rotary encoder count
volatile byte RRC = 0;    //Right rotary encoder count

boolean isMovingForward;
Movement::Movement()
{
  int i; 
  for(i = 4; i <= 7; i++){
    pinMode(i,OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(LRE), Movement::ISRLeftEncoder, HIGH);
  attachInterrupt(digitalPinToInterrupt(RRE), Movement::ISRRightEncoder, HIGH);

  isMovingForward = false;
  
  Serial.print("Movement Constructor");
}

Movement::~Movement()
{
}

void Movement::ISRLeftEncoder(){
    LRC++;
    Serial.print("Left Encoder Interrupt\n");
    if(LRC >= 8 && isMovingForward == true){
      isMovingForward = false;
      Movement::stopMovement();
    }
}

void Movement::ISRRightEncoder(){
    RRC++;
    Serial.print("Right Encoder Interrupt\n");
    if(RRC >= 8 && isMovingForward == true){
      isMovingForward = false;
      Movement::stopMovement();
    }
}

void Movement::moveForward(double distance){
  analogWrite (E1,distance);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,distance);    
  digitalWrite(M2,LOW);
  isMovingForward = true;  
}

void Movement::turn(double a) {  
  if (a == 90 && (RRC >= 8 || LRC >= 8)){
    Movement::stopMovement();
  }
}

void Movement::turnLeft() {
  Movement::turn(-90);
}

void Movement::turnRight() {
  Movement::turn(90);
}

void Movement::stopMovement(){
  digitalWrite(Enable1,LOW);   
  digitalWrite(Enable2,LOW);  
  LRC = 0;
  RRC = 0;  
}

void Movement::Test(){
  if (LRC >= 8 || RRC >= 8){
    Serial.print("I work");
  }
}


