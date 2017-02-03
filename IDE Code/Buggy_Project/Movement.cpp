#include "Movement.h"

/*
 * Speed controls
 */
int LEFT_MTR = 6;
int RIGHT_MTR = 5;

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

volatile uint8_t LRC = 0;      // Left rotary encoder count
volatile uint8_t RRC = 0;      // Right rotary encoder count

// Max of 255
unsigned int defaultRotationalSpeed = 100;
unsigned int defaultMovementSpeed   = 100;

unsigned int leftMotorSpeed;
unsigned int rightMotorSpeed;

static unsigned int timerCount = 0;

// TODO: These variables will be functions of the motor speeds
// The time (in ms) corresponding to a movement of 1 square
const unsigned int movementTimerCount = 15;

// The time (in ms) corresponding to a rotation of 90 degrees
const unsigned int turningTimerCount = 10;

movements Movement::currentMovement = IDLE;

Movement::Movement()
{
  int i;
  // Enable all the pins for the motors
  for(i = 4; i <= 7; i++){
    pinMode(i, OUTPUT); 
  }  
  
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  
  //attachInterrupt(digitalPinToInterrupt(LRE), Movement::ISRLeftEncoder, RISING);
  //attachInterrupt(digitalPinToInterrupt(RRE), Movement::ISRRightEncoder, RISING);

  currentMovement = IDLE;

  // Initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards
  pinMode(13, OUTPUT);    
  
  //Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  //Timer1.attachInterrupt(Movement::timerIsr); // attach the service routine here  
  
  Serial.println("Movement module.");

  // Make sure buggy isnt moving when we start the program
  stopMovement(); 
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
  //targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
  analogWrite (RIGHT_MTR, defaultMovementSpeed);
  digitalWrite(M1, HIGH);    
  analogWrite (LEFT_MTR, defaultMovementSpeed);    
  digitalWrite(M2, LOW);

  leftMotorSpeed  = defaultMovementSpeed;
  rightMotorSpeed = defaultMovementSpeed;

  Serial.println("Moving forward");
  // Reset the ISR timer
  timerCount = 0;
  currentMovement = FORWARD;  
}

void Movement::moveBackwards(){
  //targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
  analogWrite (RIGHT_MTR, defaultMovementSpeed);
  digitalWrite(M1, LOW);    
  analogWrite (LEFT_MTR, defaultMovementSpeed);    
  digitalWrite(M2, HIGH);

  leftMotorSpeed  = defaultMovementSpeed;
  rightMotorSpeed = defaultMovementSpeed;

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
  //targetDistance = normalisedRotationalDistance; // Equivelant to a rotation of 90 degrees
  analogWrite (RIGHT_MTR, defaultRotationalSpeed);
  digitalWrite(M1, LOW);   
  analogWrite (LEFT_MTR, defaultRotationalSpeed);    
  digitalWrite(M2, LOW);

  leftMotorSpeed  = defaultRotationalSpeed;
  rightMotorSpeed = defaultRotationalSpeed;

  Serial.println("Turning left");
  timerCount = 0;
  currentMovement = TURNING_LEFT;
}

void Movement::turnRight() {
  //targetDistance = normalisedRotationalDistance; // Equivelant to a rotation of 90 degrees
  analogWrite (RIGHT_MTR, defaultRotationalSpeed);
  digitalWrite(M1, HIGH);    
  analogWrite (LEFT_MTR, defaultRotationalSpeed);    
  digitalWrite(M2, HIGH);

  leftMotorSpeed  = defaultRotationalSpeed;
  rightMotorSpeed = defaultRotationalSpeed;  

  Serial.println("Turning right");
  timerCount = 0;
  currentMovement = TURNING_RIGHT;
}

void Movement::stopMovement(){
  Serial.println("Stopping buggy");
  
  digitalWrite(Enable1,LOW);   
  digitalWrite(Enable2,LOW); 

  currentMovement = IDLE;

  // Reset the encoder counters
  LRC = 0;
  RRC = 0;  
}

#define MAX_OUT_CHARS 16  // max nbr of characters to be sent on any one serial command
char buffer[MAX_OUT_CHARS + 1];  // buffer used to format a line (+1 is for trailing 0)

int upperLimit = 200;
int lowerLimit = 50;
int motorSensitivity = 5; // By how much do the motor values incremenent or decrement to compensate
void Movement::increaseLeftMotor(){
  sprintf(buffer,"Left speed: %d", leftMotorSpeed);  
  Serial.println(buffer);
  sprintf(buffer,"Right speed: %d", rightMotorSpeed);  
  Serial.println(buffer);
  if(leftMotorSpeed <= upperLimit){
    leftMotorSpeed += motorSensitivity;
    analogWrite (LEFT_MTR, leftMotorSpeed);
  }
}

void Movement::decreaseLeftMotor(){
  sprintf(buffer,"Left speed: %d", leftMotorSpeed);  
  Serial.println(buffer);
  sprintf(buffer,"Right speed: %d", rightMotorSpeed);  
  Serial.println(buffer);
  if(leftMotorSpeed >= lowerLimit){
    leftMotorSpeed -= motorSensitivity;
    analogWrite (LEFT_MTR, leftMotorSpeed);
  }
}

void Movement::increaseRightMotor(){
  sprintf(buffer,"Left speed: %d", leftMotorSpeed);  
  Serial.println(buffer);
  sprintf(buffer,"Right speed: %d", rightMotorSpeed);  
  Serial.println(buffer);
  if(rightMotorSpeed <= upperLimit){
    rightMotorSpeed += motorSensitivity;
    analogWrite (RIGHT_MTR, rightMotorSpeed);
  }
}

void Movement::decreaseRightMotor(){
  sprintf(buffer,"Left speed: %d", leftMotorSpeed);  
  Serial.println(buffer);
  sprintf(buffer,"Right speed: %d", rightMotorSpeed);  
  Serial.println(buffer);
  if(rightMotorSpeed >= lowerLimit){
    rightMotorSpeed -= motorSensitivity;
    analogWrite (RIGHT_MTR, rightMotorSpeed);
  }
}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void Movement::timerIsr()
{
    // If the buggy is *moving* and the timer count has exceeded
    if( (currentMovement == FORWARD || currentMovement == BACKWARDS) && timerCount > movementTimerCount){
      timerCount = 0;
      Serial.println("Finished moving");
      stopMovement();
    }

    // If the buggy is *turning* and the timer count has exceeded
    if( (currentMovement == TURNING_LEFT || currentMovement == TURNING_RIGHT) && timerCount > turningTimerCount){
      timerCount = 0;
      Serial.println("Finished turning");
      stopMovement();
    }
    
    timerCount++;
}

/*
 * Convert the movement enum to a string (for debugging)
 */
String Movement::getMovement(movements cm){
  switch(cm){
    case BACKWARDS:
      return "BACKWARDS";
      break;
    case FORWARD:
      return "FORWARDS";
      break;
    case TURNING_LEFT:
      return "TURNING_LEFT";
      break;
    case TURNING_RIGHT:
      return "TURNING_RIGHT";
      break;
    case IDLE:
      return "IDLE";
      break;
    default:
      return "UNKNOWN";
  }
}


