#include "Movement.h"

//#include "Timer.h" // For the timer interrupts

int E1 = 5;     // M1 Speed Control
int E2 = 6;     // M2 Speed Control
int M1 = 4;     // M1 Direction Control
int M2 = 7;     // M1 Direction Control

// assuming pwm movement 
const int Enable1 = 5;      // Motor1 Speed Control
const int Enable2 = 6;      // Motor2 Speed Control
const int Motor1  = 4;      // Motor1 Direction Control
const int Motor2  = 7;      // Motor1 Direction Control
const byte LRE    = 6;      // Left rotary encoder pin holder value
const byte RRE    = 5;      // Right rotary encoder pin holder value
volatile byte LRC = 0;      // Left rotary encoder count
volatile byte RRC = 0;      // Right rotary encoder count

enum movements {FORWARD, BACKWARDS, TURNING_LEFT, TURNING_RIGHT, IDLE};
movements currentMovement;

unsigned int normalisedRotationalDistance = 100;    // The encoder distance to correspond to a 90 degrees turn
unsigned int normalisedMovementDistance   = 255;    // The encoder distance to correspond to moving forward 1 cell
unsigned int targetDistance;

Movement::Movement()
{
  int i; 
  for(i = 4; i <= 7; i++){
    pinMode(i,OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(LRE), Movement::ISRLeftEncoder, HIGH);
  attachInterrupt(digitalPinToInterrupt(RRE), Movement::ISRRightEncoder, HIGH);

  currentMovement = IDLE;
  targetDistance  = -1;
  
  Serial.print("Movement Constructor");
}

Movement::~Movement()
{
}

void Movement::ISRLeftEncoder(){
    LRC++;
    Serial.print("Left Encoder Interrupt\n");
    
    // If the buggy is moving/turning and has exceeded its target distance
    if(LRC >= targetDistance && currentMovement != IDLE){
      currentMovement = IDLE;
      Movement::stopMovement();
    }
}

void Movement::ISRRightEncoder(){
    RRC++;
    Serial.print("Right Encoder Interrupt\n");
    
    // If the buggy is moving/turning and has exceeded its target distance
    if(RRC >= targetDistance && currentMovement != IDLE){
      currentMovement = IDLE;
      Movement::stopMovement();
    }
}

void Movement::moveForward(){
  targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
  analogWrite (E1,targetDistance);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,targetDistance);    
  digitalWrite(M2,LOW);
  
  currentMovement = FORWARD;  
}

void Movement::moveBackwards(){
  targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
  analogWrite (E1,targetDistance);
  digitalWrite(M1,LOW);    
  analogWrite (E2,targetDistance);    
  digitalWrite(M2,HIGH);
  
  currentMovement = BACKWARDS;
}

void Movement::turn(double degrees) {  
  /*
   * Yet to be implemented
   */
}

void Movement::turnLeft() {
  targetDistance = normalisedRotationalDistance; // Equivelant to a rotation of 90 degrees
  analogWrite (E1,targetDistance);
  digitalWrite(M1,LOW);   
  analogWrite (E2,targetDistance);    
  digitalWrite(M2,LOW);
  
  currentMovement = TURNING_LEFT;
}

void Movement::turnRight() {
  targetDistance = normalisedRotationalDistance; // Equivelant to a rotation of 90 degrees
  analogWrite (E1,targetDistance);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,targetDistance);    
  digitalWrite(M2,HIGH);
  
  currentMovement = TURNING_RIGHT;
}

void Movement::stopMovement(){
  digitalWrite(Enable1,LOW);   
  digitalWrite(Enable2,LOW);  

  // Reset the encoder counters
  LRC = 0;
  RRC = 0;  
}

void Movement::Test(){
  if (LRC >= 8 || RRC >= 8){
    Serial.print("I work");
  }
}


