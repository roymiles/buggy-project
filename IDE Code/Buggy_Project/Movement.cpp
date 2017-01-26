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

/*
 * The encoder pins are 2,3
 */
const byte LRE    = 1;      // Left rotary encoder pin holder value
const byte RRE    = 0;      // Right rotary encoder pin holder value

volatile byte LRC = 0;      // Left rotary encoder count
volatile byte RRC = 0;      // Right rotary encoder count

// Max of 255
unsigned int normalisedRotationalDistance = 100;    // The encoder distance to correspond to a 90 degrees turn
unsigned int normalisedMovementDistance   = 100;    // The encoder distance to correspond to moving forward 1 cell
unsigned int targetDistance;

static unsigned int timerCount = 0;

movements Movement::currentMovement = IDLE;

Movement::Movement()
{
  int i; 
  for(i = 4; i <= 7; i++){
    pinMode(i,OUTPUT);
  }

  //attachInterrupt(digitalPinToInterrupt(LRE), Movement::ISRLeftEncoder, RISING);
  //attachInterrupt(digitalPinToInterrupt(RRE), Movement::ISRRightEncoder, RISING);

  currentMovement = IDLE;
  targetDistance  = -1;

  // Initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards
  pinMode(13, OUTPUT);    
  
  Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt(Movement::timerIsr); // attach the service routine here  
  
  Serial.println("Movement module.");
}

Movement::~Movement()
{
}

void Movement::ISRLeftEncoder(){
    LRC++;
    Serial.println("Left encoder interrupt triggered");
    
    // If the buggy is moving/turning and has exceeded its target distance
    /*if(LRC >= targetDistance && currentMovement != IDLE){
      currentMovement = IDLE;
      Movement::stopMovement();
    }*/
}

void Movement::ISRRightEncoder(){
    RRC++;
    Serial.println("Right encoder interrupt triggered");
    
    // If the buggy is moving/turning and has exceeded its target distance
    /*if(RRC >= targetDistance && currentMovement != IDLE){
      currentMovement = IDLE;
      Movement::stopMovement();
    }*/
}

void Movement::moveForward(){
  targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
  analogWrite (E1,targetDistance);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,targetDistance);    
  digitalWrite(M2,LOW);

  Serial.println("Moving forward");
  // Reset the ISR timer
  timerCount = 0;
  currentMovement = FORWARD;  
}

void Movement::moveBackwards(){
  targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
  analogWrite (E1,targetDistance);
  digitalWrite(M1,LOW);    
  analogWrite (E2,targetDistance);    
  digitalWrite(M2,HIGH);

  Serial.println("Moving backwards");
  timerCount = 0;
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

  Serial.println("Turning left");
  timerCount = 0;
  currentMovement = TURNING_LEFT;
}

void Movement::turnRight() {
  targetDistance = normalisedRotationalDistance; // Equivelant to a rotation of 90 degrees
  analogWrite (E1,targetDistance);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,targetDistance);    
  digitalWrite(M2,HIGH);

  Serial.println("Turning right");
  timerCount = 0;
  currentMovement = TURNING_RIGHT;
}

void Movement::stopMovement(){
  digitalWrite(Enable1,LOW);   
  digitalWrite(Enable2,LOW); 

  currentMovement = IDLE;

  // Reset the encoder counters
  LRC = 0;
  RRC = 0;  
}

/*void Movement::Test(){
  if (LRC >= 8 || RRC >= 8){
    Serial.println("I work");
  }
}*/

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void Movement::timerIsr()
{
    // If the buggy is moving/turning and the timer count has exceeded
    if(currentMovement != IDLE && timerCount > 10){
      timerCount = 0;
      Serial.println("Finished moving");
      stopMovement();
    }
    
    timerCount++;
    // If 1 second has passed
    //if(timerCount % 10 == 0){
     // Serial.println("1 second has passed");
      //timerCount = 0;
    //}
}


