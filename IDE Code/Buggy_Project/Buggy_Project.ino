/**
    Purpose: This is the entry point for the application
       please don't bloat this file too much

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "UltraSonicSensor.h"
#include "Movement.h"
#include "PathFinding.h"
#include "MazeLayout.h"

UltraSonicSensor *uss;
Movement *m;
PathFinding *p;
MazeLayout *ml;

//Standard PWM DC control
/*int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control*/

///For previous Romeo, please use these pins.
//int E1 = 6;     //M1 Speed Control
//int E2 = 9;     //M2 Speed Control
//int M1 = 7;    //M1 Direction Control
//int M2 = 8;    //M1 Direction Control


/*
 * The following code is a working copy of motor movement, don't delete
 * 
void stop(void)                    //Stop
{
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW);      
}   
void advance(char a,char b)          //Move forward
{

  analogWrite (E1,a);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}  
void back_off (char a,char b)          //Move backward
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);   
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}*/


void setup() {
  // put your setup code here, to run once:
  //interrupts();

  int i;
  for(i=4;i<=7;i++){
    pinMode(i, OUTPUT);  
  }

  // For debugging
  Serial.begin(9600);      // open the serial port at 9600 bps:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("Run keyboard control");
  
  Serial.print("Instantiating Modules...\n");

  // Create the objects we need, DONT create more than one copy.
  uss = new UltraSonicSensor();
  m = new Movement();
  p = new PathFinding();
  //MazeLayout::test();
}


double distance;
void loop() {
  // put your main code here, to run repeatedly:
  m->Test();
  //distance = uss->getDistanceToNearestObject();

  // Delay 50ms
  //delay(50);

  if(Serial.available()){
    char val = Serial.read();
    if(val != -1)
    {
      switch(val)
      {
      case 'w'://Move Forward
        //advance (255,255);   //move forward in max speed
        m->moveForward();
        break;
      case 's'://Move Backward
        //back_off (255,255);   //move back in max speed
        m->moveBackwards();
        break;
      case 'a'://Turn Left
        //turn_L (100,100);
        m->turnLeft();
        break;       
      case 'd'://Turn Right
        //turn_R (100,100);
        m->turnRight();
        break;
      case 'z':
        Serial.println("Hello");
        break;
      case 'x':
        m->stopMovement();
        break;
      }
    }
    else{
      m->stopMovement();  
    }
  }

  
  
}
