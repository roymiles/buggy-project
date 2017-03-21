#include "Movement.h"

/*
 * Speed controls
 */
int LEFT_MTR = 5;
int RIGHT_MTR = 6;

/*
 * Direction controls
 */
int M1 = 4;     // M1 Direction Control
int M2 = 7;     // M1 Direction Control

/*
 * The encoder pins are 2,3
 */
//const byte LRE    = 1;      // Left rotary encoder pin holder value
//const byte RRE    = 0;      // Right rotary encoder pin holder value
//
//volatile uint8_t LRC = 0;      // Left rotary encoder count
//volatile uint8_t RRC = 0;      // Right rotary encoder count

// Max of 255
unsigned int defaultRotationalSpeed = 120;
unsigned int defaultMovementSpeed   = 120;
unsigned int defaultSkidSpeed       = 120;

unsigned int leftMotorSpeed;
unsigned int rightMotorSpeed;

static unsigned int timerCount = 0;

#define MAX_OUT_CHARS 16  // max nbr of characters to be sent on any one serial command
char buffer[MAX_OUT_CHARS + 1];  // buffer used to format a line (+1 is for trailing 0)

//int upperLimit = 150;
//int lowerLimit = 60;
//int motorSensitivity = 20; // By how much do the motor values incremenent or decrement to compensate

// TODO: These variables will be functions of the motor speeds
// The time (in tenths of ms) corresponding to a movement of 1 square
const unsigned int movementTimerCount = 150;
const unsigned int movementToggleCount = 10; // Every 0.01 seconds

// The time (in ms) corresponding to a rotation of 90 degrees
const unsigned int turningTimerCount = 12;
  
movements Movement::currentMovement = IDLE;
movementCompensation Movement::currentMovementCompensation = ON_TRACK;

bool Movement::isWiggling = false;

Movement::Movement()
{
  // Enable all the pins for the motors (reduce local variable usage by not using a for loop)
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT); 
  
  //attachInterrupt(digitalPinToInterrupt(LRE), Movement::ISRLeftEncoder, RISING);
  //attachInterrupt(digitalPinToInterrupt(RRE), Movement::ISRRightEncoder, RISING);

  currentMovement = IDLE;
  isWiggling      = false;  
  
  Timer1.initialize(10000); // set a timer of length 10000 microseconds (0.01 seconds)
  Timer1.attachInterrupt(Movement::timerIsr); // attach the service routine here  
  
  Serial.println("Movement module.");

  // Make sure buggy isnt moving when we start the program
  stopMovement(); 
}

Movement::~Movement()
{
}

//void Movement::ISRLeftEncoder(){
//    LRC++;
//    Serial.println("Left encoder interrupt triggered");
//    
//    // If the buggy is moving/turning and has exceeded its target distance
//    /*if(LRC >= targetDistance && currentMovement != IDLE){
//      currentMovement = IDLE;
//      Movement::stopMovement();
//    }*/
//}
//
//void Movement::ISRRightEncoder(){
//    RRC++;
//    Serial.println("Right encoder interrupt triggered");
//    
//    // If the buggy is moving/turning and has exceeded its target distance
//    /*if(RRC >= targetDistance && currentMovement != IDLE){
//      currentMovement = IDLE;
//      Movement::stopMovement();
//    }*/
//}


/**
 * Turn on the motors to go forward and reset the timer count
 */
void Movement::moveForward(){
  //targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
//  analogWrite (RIGHT_MTR, defaultMovementSpeed);
//  digitalWrite(M1, HIGH);    
//  analogWrite (LEFT_MTR, defaultMovementSpeed);    
//  digitalWrite(M2, LOW);

  analogWrite (RIGHT_MTR, defaultMovementSpeed);
  digitalWrite(M1, HIGH);    
  analogWrite (LEFT_MTR, defaultMovementSpeed);    
  digitalWrite(M2, HIGH);

  leftMotorSpeed  = defaultMovementSpeed;
  rightMotorSpeed = defaultMovementSpeed;

  Serial.println("Moving forward");
  // Reset the ISR timer
  timerCount = 0;
  currentMovement = FORWARD;  
}

/**
 * Turn on the motors to go backwards and reset the timer count
 */
void Movement::moveBackwards(){
  //targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
//  analogWrite (RIGHT_MTR, defaultMovementSpeed);
//  digitalWrite(M1, LOW);    
//  analogWrite (LEFT_MTR, defaultMovementSpeed);    
//  digitalWrite(M2, HIGH);

  analogWrite (RIGHT_MTR, defaultMovementSpeed);
  digitalWrite(M1, LOW);    
  analogWrite (LEFT_MTR, defaultMovementSpeed);    
  digitalWrite(M2, LOW);

  leftMotorSpeed  = defaultMovementSpeed;
  rightMotorSpeed = defaultMovementSpeed;

  Serial.println("Moving backwards");
  timerCount = 0;
  currentMovement = BACKWARDS;
}


/**
 * Turn on the motors to turn left and reset the timer count
 */
void Movement::turnLeft() {
  //targetDistance = normalisedRotationalDistance; // Equivelant to a rotation of 90 degrees
//  analogWrite (RIGHT_MTR, defaultRotationalSpeed);
//  digitalWrite(M1, LOW);   
//  analogWrite (LEFT_MTR, defaultRotationalSpeed);    
//  digitalWrite(M2, LOW);

  analogWrite (RIGHT_MTR, defaultMovementSpeed);
  digitalWrite(M1, LOW);    
  analogWrite (LEFT_MTR, defaultMovementSpeed);    
  digitalWrite(M2, HIGH);

  leftMotorSpeed  = defaultRotationalSpeed;
  rightMotorSpeed = defaultRotationalSpeed;

  Serial.println("Turning left");
  timerCount = 0;
  currentMovement = TURNING_LEFT;
}

/**
 * Turn on the motors to go forward and reset the timer count
 */
unsigned int leftMotorCompensation; 
void Movement::turnRight() {
  //targetDistance = normalisedRotationalDistance; // Equivelant to a rotation of 90 degrees

  // The left wiggling is not as strong as the right wiggle. So can compensate by adjusting the values
  if(isWiggling == true){
    leftMotorCompensation = defaultRotationalSpeed;
  }else{
    leftMotorCompensation = defaultRotationalSpeed;
  }
  
//  analogWrite (RIGHT_MTR, leftMotorCompensation);
//  digitalWrite(M1, HIGH);    
//  analogWrite (LEFT_MTR, leftMotorCompensation);    
//  digitalWrite(M2, HIGH);

  analogWrite (RIGHT_MTR, defaultMovementSpeed);
  digitalWrite(M1, HIGH);    
  analogWrite (LEFT_MTR, defaultMovementSpeed);    
  digitalWrite(M2, LOW);
  
  leftMotorSpeed  = defaultRotationalSpeed;
  rightMotorSpeed = defaultRotationalSpeed;  

  Serial.println("Turning right");
  timerCount = 0;
  currentMovement = TURNING_RIGHT;
}

/**
 * Stop the motors and put the buggy in an IDLE state
 */
void Movement::stopMovement(){
  Serial.println("Stopping buggy");
  
  digitalWrite(LEFT_MTR,LOW);   
  digitalWrite(RIGHT_MTR,LOW); 

  Movement::currentMovement = IDLE;

  // Reset the encoder counters
  // LRC = 0;
  // RRC = 0;  

  delay(100); // Allow things to settle
}


/**
 * The following compensation functions are no longer used
 */
//void Movement::increaseLeftMotor(){
//  if(leftMotorSpeed <= upperLimit){
//    leftMotorSpeed += motorSensitivity;
//    analogWrite (LEFT_MTR, leftMotorSpeed);
//  }
//}
//
//void Movement::decreaseLeftMotor(){
//  analogWrite (LEFT_MTR, defaultSkidSpeed);
//  /*sprintf(buffer,"Left speed: %d", leftMotorSpeed);  
//  Serial.println(buffer);
//  sprintf(buffer,"Right speed: %d", rightMotorSpeed);  
//  Serial.println(buffer);
//  if(leftMotorSpeed >= lowerLimit){
//    leftMotorSpeed -= motorSensitivity;
//    analogWrite (LEFT_MTR, leftMotorSpeed);
//  }*/
//}
//
//void Movement::increaseRightMotor(){
//  if(rightMotorSpeed <= upperLimit){
//    rightMotorSpeed += motorSensitivity;
//    analogWrite (RIGHT_MTR, rightMotorSpeed);
//  }
//}
//
//void Movement::decreaseRightMotor(){
//  analogWrite (RIGHT_MTR, defaultSkidSpeed);
//  /*sprintf(buffer,"Left speed: %d", leftMotorSpeed);  
//  Serial.println(buffer);
//  sprintf(buffer,"Right speed: %d", rightMotorSpeed);  
//  Serial.println(buffer);
//  if(rightMotorSpeed >= lowerLimit){
//    rightMotorSpeed -= motorSensitivity;
//    analogWrite (RIGHT_MTR, rightMotorSpeed);
//  }*/
//}

/**
 * Functions to enable and disable the motors
 */
void Movement::enableLeftMotor(){
  analogWrite (LEFT_MTR, defaultSkidSpeed);
}

void Movement::disableLeftMotor(){
  analogWrite (LEFT_MTR, 0);
}

void Movement::enableRightMotor(){
  analogWrite (RIGHT_MTR, defaultSkidSpeed);
}

void Movement::disableRightMotor(){
  analogWrite (RIGHT_MTR, 0);
}


/// --------------------------
/// Custom ISR Timer Routine
/// If the buggy is moving (not wiggling) turn the motors on and off (to compensate for low torque)
/// --------------------------
bool motorToggle = false; // Toggle the motors on and off to reduce overshoot due to high speed
void Movement::timerIsr()
{
    // If the buggy is *moving* and the timer count has exceeded
   /*if( (currentMovement == FORWARD || currentMovement == BACKWARDS) && timerCount > movementTimerCount){
      timerCount = 0;
      Serial.println("Finished moving - timer interrupt");
      stopMovement();
    }*/

    // If the buggy is *turning* and the timer count has exceeded
    /*if( (currentMovement == TURNING_LEFT || currentMovement == TURNING_RIGHT || currentMovement == FORWARD || currentMovement == BACKWARDS) && timerCount > turningTimerCount){
      timerCount = 0;
      Serial.println("Finished turning - timer interrupt");
      stopMovement();
    }*/

    // If the buggy is moving keep stopping and starting the motors
    if(currentMovement != IDLE  /*&& isWiggling == false /*(currentMovement == FORWARD || currentMovement == BACKWARDS)*/ && timerCount > movementToggleCount){
      timerCount = 0;
      if(motorToggle){
        motorToggle = false;

        switch(currentMovementCompensation){
          case ON_TRACK:
            analogWrite (RIGHT_MTR, defaultMovementSpeed);
            analogWrite (LEFT_MTR, defaultMovementSpeed);
            break;
          case COMPENSATING_LEFT:
            analogWrite (RIGHT_MTR, defaultSkidSpeed);
            analogWrite (LEFT_MTR, 0);
            break;
          case COMPENSATING_RIGHT:
            analogWrite (RIGHT_MTR, 0);
            analogWrite (LEFT_MTR, defaultSkidSpeed);
            break;
        }
      }else{
        motorToggle = true;
        // Serial.println("Toggling off");
        analogWrite (RIGHT_MTR, 0);
        analogWrite (LEFT_MTR, 0);
      }
      
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

static String Movement::movementStateToString(movementCompensation ms){
  switch(ms){
    case ON_TRACK:
      return "ON_TRACK";
      break;
    case COMPENSATING_LEFT:
      return "COMPENSATING_LEFT";
      break;
    case COMPENSATING_RIGHT:
      return "COMPENSATING_RIGHT";
      break;
    default:
      return "UNKNOWN MOVEMENT STATE";
  }
}


