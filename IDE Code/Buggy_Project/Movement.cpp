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

unsigned int leftMotorSpeed;
unsigned int rightMotorSpeed;

static unsigned int timerCount = 0;

#define MAX_OUT_CHARS 16  // max nbr of characters to be sent on any one serial command
char buffer[MAX_OUT_CHARS + 1];  // buffer used to format a line (+1 is for trailing 0)

// The time (in tenths of ms) corresponding to a movement of 1 square
const unsigned int movementTimerCount = 150;
const unsigned int movementToggleCount = 5; // Every 0.01 seconds

// The time (in ms) corresponding to a rotation of 90 degrees
const unsigned int turningTimerCount = 12;
  
movements Movement::currentMovement = IDLE;
movementCompensation Movement::currentMovementCompensation = ON_TRACK;

bool Movement::isWiggling = false;
bool Movement::isCalibrating = true;


// Max of 255
unsigned int Movement::defaultRotationalSpeed = 130;//90;
unsigned int Movement::defaultMovementSpeed   = 130;//90;
unsigned int Movement::defaultSkidSpeed       = 120;

/**
 * This is the constructor of the Movement object.
 * It is responsible for initialising the timer interrupts and
 * the output pins for the motors
 */
Movement::Movement()
{
  // Enable all the pins for the motors (reduce local variable usage by not using a for loop)
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT); 

  currentMovement = IDLE;
  Movement::isWiggling = false;  
  
  Timer1.initialize(10000); // set a timer of length 10000 microseconds (0.01 seconds)
  Timer1.attachInterrupt(Movement::timerIsr); // attach the service routine here  
  
  Serial.println("Movement module.");

  // Make sure buggy isnt moving when we start the program
  stopMovement(); 
}

/**
 * Turn on the motors to go forward and reset the timer count
 */
void Movement::moveForward(){
  //targetDistance = normalisedMovementDistance; // Equivelant to a movement of 1 cell
  analogWrite (RIGHT_MTR, Movement::defaultMovementSpeed);
  digitalWrite(M1, LOW);    
  analogWrite (LEFT_MTR, Movement::defaultMovementSpeed);    
  digitalWrite(M2, HIGH);

  leftMotorSpeed  = Movement::defaultMovementSpeed;
  rightMotorSpeed = Movement::defaultMovementSpeed;

//  Serial.println("Moving forward");
  // Reset the ISR timer
  timerCount = 0;
  currentMovement = FORWARD;  
}

/**
 * Turn on the motors to go backwards and reset the timer count
 */
void Movement::moveBackwards(){

  unsigned int sp = (Movement::defaultMovementSpeed - 40);
  analogWrite(RIGHT_MTR, sp);
  digitalWrite(M1, HIGH);    
  analogWrite(LEFT_MTR, sp);    
  digitalWrite(M2, LOW);

  leftMotorSpeed  = Movement::defaultMovementSpeed;
  rightMotorSpeed = Movement::defaultMovementSpeed;

//  Serial.println("Moving backwards");
  timerCount = 0;
  currentMovement = BACKWARDS;
}


/**
 * Turn on the motors to turn left and reset the timer count
 */
void Movement::turnLeft() {
  analogWrite (RIGHT_MTR, Movement::defaultRotationalSpeed);
  digitalWrite(M1, HIGH);   
  analogWrite (LEFT_MTR, Movement::defaultRotationalSpeed);    
  digitalWrite(M2, HIGH);

  leftMotorSpeed  = Movement::defaultRotationalSpeed;
  rightMotorSpeed = Movement::defaultRotationalSpeed;

//  Serial.println("Turning left");
  timerCount = 0;
  currentMovement = TURNING_LEFT;
}

/**
 * Turn on the motors to go forward and reset the timer count
 */
unsigned int leftMotorCompensation; 
void Movement::turnRight() {
  // The left wiggling is not as strong as the right wiggle. So can compensate by adjusting the values
  if(Movement::isWiggling == true){
    leftMotorCompensation = Movement::defaultRotationalSpeed;
  }else{
    leftMotorCompensation = Movement::defaultRotationalSpeed;
  }
  
  analogWrite (RIGHT_MTR, leftMotorCompensation);
  digitalWrite(M1, LOW);    
  analogWrite (LEFT_MTR, leftMotorCompensation);    
  digitalWrite(M2, LOW);

//  analogWrite (RIGHT_MTR, defaultRotationalSpeed);
//  digitalWrite(M1, HIGH);    
//  analogWrite (LEFT_MTR, defaultRotationalSpeed);    
//  digitalWrite(M2, LOW);
  
  leftMotorSpeed  = Movement::defaultRotationalSpeed;
  rightMotorSpeed = Movement::defaultRotationalSpeed;  

//  Serial.println("Turning right");
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

  Serial.println(F("-------------------------"));
  Serial.println(F("-------- STOPPED --------"));
  Serial.println(F("-------------------------"));

  delay(100); // Allow things to settle
}

/**
 * The following functions enable and disable the motors
 */
void Movement::enableLeftMotor(){
  analogWrite (LEFT_MTR, Movement::defaultSkidSpeed);
}

void Movement::disableLeftMotor(){
  analogWrite (LEFT_MTR, 0);
}

void Movement::enableRightMotor(){
  analogWrite (RIGHT_MTR, Movement::defaultSkidSpeed);
}

void Movement::disableRightMotor(){
  analogWrite (RIGHT_MTR, 0);
}


/**
 *  Custom ISR Timer Routine (static function)
 *  The buggy can overshoot due to high speeds but using a low speed
 *  does not generate enough torque to move the buggy
 *  
 *  The solution is to use a high speed but to toggle them on and off periodically
 *  The toggling is only carried out for forward and backward movements
 */
bool motorToggle = false; // Toggle the motors on and off to reduce overshoot due to high speed
int lightToggle = HIGH;

void Movement::timerIsr()
{
    // If the buggy is moving keep stopping and starting the motors
    if(currentMovement != IDLE  && Movement::isWiggling == false /*&& (currentMovement == FORWARD || currentMovement == BACKWARDS)*/ && timerCount > movementToggleCount){
      timerCount = 0;
      if(motorToggle){
        motorToggle = false;

        unsigned int sp = Movement::defaultMovementSpeed;
        if(currentMovement == BACKWARDS){
          sp = (Movement::defaultMovementSpeed - 40);
        }

        switch(currentMovementCompensation){
          case ON_TRACK:
            analogWrite (RIGHT_MTR, sp);
            analogWrite (LEFT_MTR, sp);
            break;
          case COMPENSATING_LEFT:
            analogWrite (RIGHT_MTR, sp);
            analogWrite (LEFT_MTR, sp-80); // 80
            break;
          case COMPENSATING_RIGHT:
            analogWrite (RIGHT_MTR, sp-80); // 80
            analogWrite (LEFT_MTR, sp);
            break;
        }
      }else{
        motorToggle = true;
        // Serial.println("Toggling off");
        analogWrite (RIGHT_MTR, 0);
        analogWrite (LEFT_MTR, 0);
      }
      
//    }else if(Movement::isWiggling == true && timerCount > wiggleDelay){
//      timerCount = 0; // Reset timer
//      wiggleToggle = 1;
      
    }else if((timerCount % 100) == 0 && Movement::isCalibrating == true){
      Serial.println(F("Calibrating..."));
      digitalWrite(A3, lightToggle);
      digitalWrite(A4, lightToggle);
      digitalWrite(A5, lightToggle);

      lightToggle = ((lightToggle == HIGH) ? LOW : HIGH);
    }
    
    timerCount++;
    
}

/*
 * Convert the movement enum to a string
 * @param cm, movements enum
 * @return the string representation of the movement
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


