/**
    Communication.cpp
    Purpose: This class is used to communicate to the SLAVE device in
    order to transmit and recieve data from the basestation

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "Arduino.h"
#include "Communication.h"

Communication::Communication()
{
  // Start the I2C Bus as Master
  Wire.begin();
}


Communication::~Communication()
{
}

int Communication::dataToCommand(int d){
  return  d + (I2C_MAX+1);
}

int Communication::commandToData(I2C_COMMAND cmd){
  return cmd  - (I2C_MAX+1);
} 


void Communication::sendPosition(int x, int y, I2C_COMMAND orientation){
  /*
   * Master wants to transmit current buggy position
   */
  Wire.beginTransmission(SLAVE_ID); // transmit to device #44 (0x2c)
                              // device address is specified in datasheet
  Serial.print("Transmitting: ");
  Serial.println(commandToString(I2C_COMMAND::MASTER_SEND_POSITION));                            
  Wire.write(I2C_COMMAND::MASTER_SEND_POSITION);  // sends value byte  
  Wire.endTransmission();                         // stop transmitting
  delay(500);

  /*
   * Send current x-coordinate
   */
  Wire.beginTransmission(SLAVE_ID);
  Serial.print("Transmitting: ");
  Serial.println(x);                            
  Wire.write(dataToCommand(x)); 
  Wire.endTransmission();
  delay(1000);
  
  /*
   * Send current y-coordinate
   */
  Wire.beginTransmission(SLAVE_ID);
  Serial.print("Transmitting: ");
  Serial.println(y);                            
  Wire.write(dataToCommand(y)); 
  Wire.endTransmission();
  delay(1000);
  
  /*
   * Send current orientation
   */
  Wire.beginTransmission(SLAVE_ID);
  Serial.print("Transmitting: ");
  Serial.println(commandToString(orientation));                            
  Wire.write(orientation); 
  Wire.endTransmission();
  delay(1000);
}

I2C_COMMAND Communication::getNextMovement(){
  /*
   * Request new movement from slave
   * FORWARD, BACKWARDS, LEFT or RIGHT
   */
  Serial.println("Requesting 2 bytes from slave");
  Wire.requestFrom(SLAVE_ID, 2);    // request 2 bytes from slave device #44
  
  char recievedVal[2];
  uint8_t i = 0;
  Serial.print("Raw: ");
  while (Wire.available()) {          // slave may send less than requested
    char c = Wire.read();             // receive a byte as character
    recievedVal[i] = c;
    Serial.print(c);
    i++;
  }
  Serial.println();

  /*
   * Note to self: double check how this works!
   */
  I2C_COMMAND cmd = static_cast<I2C_COMMAND>(recievedVal[0]); //(recievedVal[1] << 8) | recievedVal[0];
  Serial.print("Recieved: ");
  Serial.println(commandToString(cmd));
  delay(500);
}

String Communication::commandToString(I2C_COMMAND cmd){
  switch(cmd){
    case MASTER_IDLE:
      return "MASTER_IDLE";
      break;
     case MASTER_MOVING:
      return "MASTER_MOVING";
      break;
     case MASTER_DOCKED:
      return "MASTER_DOCKED";
      break;
     case MASTER_ADJUSTING:
      return "MASTER_ADJUSTING";
      break;
     case MASTER_SEND_POSITION:
      return "MASTER_SEND_POSITION";
      break;
  
     case SLAVE_IDLE:
      return "SLAVE_IDLE";
      break;
     case SLAVE_BUSY:
      return "SLAVE_BUST";
      break;
     case SLAVE_DOCKED:
      return "SLAVE_DOCKED";
      break;
  
    case I2C_NORTH:
      return "NORTH";
      break;
    case I2C_EAST:
      return "EAST";
      break;
    case I2C_SOUTH:
      return "SOUTH";
      break;
    case I2C_WEST:
      return "WEST";
      break;      
  
    case DEFAULT_SLAVE_STATE:
      return "DEFAULT_SLAVE_STATE";
      break;
    case AWAITING_X:
      return "AWAITING_X";
      break;
    case AWAITING_Y:
      return "AWAITING_Y";
      break;
    case AWAITING_ORIENTATION:
      return "AWAITING_ORIENTATION";
      break;
  
    case I2C_BACKWARDS:
      return "BACKWARDS";
      break;
    case I2C_FORWARD:
      return "FORWARDS";
      break;
    case I2C_TURN_LEFT:
      return "TURN_LEFT";
      break;
    case I2C_TURN_RIGHT:
      return "TURN_RIGHT";
      break;
    
    default:
      return "DATA or UNKNOWN";
  } 
}
