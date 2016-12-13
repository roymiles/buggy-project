#include "Movement.h"

// assuming pwm movement 
const int Enable1 = 5;     //Motor1 Speed Control
const int Enable2 = 6;     //Motor2 Speed Control
const int Motor1 = 4;    //Motor1 Direction Control
const int Motor2 = 7;    //Motor1 Direction Control
const byte LRE = 0; //Left rotary encoder pin holder value
const byte RRE = 1; //Right rotary encoder pin holder value
volatile byte RRC = 0;// right rotary encoder count
volatile byte LRC = 0;// left rotary encoder count

Movement::Movement()
{
  int i;
  for(i=4;i<=7;i++){
    pinMode(i,OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(LRE), Movement::ISRLeftEncoder, HIGH);
  attachInterrupt(digitalPinToInterrupt(RRE), Movement::ISRRightEncoder, HIGH);

}

Movement::~Movement()
{
}
void Movement::ISRLeftEncoder(){
    LRC++;
}
void Movement::ISRRightEncoder(){
    RRC++;
}
void Movement::moveForward(double distance){
  
}

void Movement::turn(double a) {
  if (a == 90 && RRC>8||LRC>=8) {
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
  
};


