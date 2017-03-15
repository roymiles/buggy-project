#include <Wire.h>
// #include "EasyTransferI2C.h"

uint16_t randomNumber;
enum I2C_COMMAND : uint16_t // Used for static conversions
{
  MASTER_IDLE = 0, 
  MASTER_MOVING = 1,
  MASTER_DOCKED = 2, 
  MASTER_ADJUSTING = 3, 
  MASTER_SEND_POSITION = 4,
  
  SLAVE_IDLE = 5,
  SLAVE_BUSY = 6,
  SLAVE_DOCKED = 7,
  
  NORTH = 8,
  EAST = 9,
  SOUTH = 10,
  WEST = 11,
  
  NA = 12,
  AWAITING_X = 13,
  AWAITING_Y = 14,
  AWAITING_ORIENTATION = 15,

  MIN = 0,
  MAX = 15

  /*
   * Anything higher is interpreted as a data value
   */
};

uint16_t dataToCommand(uint16_t d){ 
  return  d + (MAX+1);
}

uint16_t commandToData(I2C_COMMAND cmd){
  return cmd  - (MAX+1);
}

int val = 0;
void setup() {
  Serial.println("Master setup");
  // Start the I2C Bus as Master
  Wire.begin();  
}
void loop() {
  /*
   * Master wants to transmit current buggy position
   */
  Wire.beginTransmission(44); // transmit to device #44 (0x2c)
                              // device address is specified in datasheet
  Serial.print("Transmitting: ");
  Serial.println(commandToString(I2C_COMMAND::MASTER_SEND_POSITION));                            
  Wire.write(I2C_COMMAND::MASTER_SEND_POSITION); // sends value byte  
  Wire.endTransmission();           // stop transmitting
  delay(500);

  /*
   * Send current x-coordinate
   */
  Wire.beginTransmission(44);
  Serial.print("Transmitting: ");
  //generate a random number
  randomNumber = 20;
  Serial.println(randomNumber);                            
  Wire.write(dataToCommand(randomNumber)); 
  Wire.endTransmission();
  delay(1000);
  
  /*
   * Send current y-coordinate
   */
  Wire.beginTransmission(44);
  Serial.print("Transmitting: ");
  //generate a random number
  randomNumber = 18;
  Serial.println(randomNumber);                            
  Wire.write(dataToCommand(randomNumber)); 
  Wire.endTransmission();
  delay(1000);
  
  /*
   * Send current orientation
   */
  Wire.beginTransmission(44);
  Serial.print("Transmitting: ");
  Serial.println(commandToString(I2C_COMMAND::WEST));                            
  Wire.write(I2C_COMMAND::WEST); 
  Wire.endTransmission();
  delay(1000);
}

String commandToString(I2C_COMMAND cmd){
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
  
    case NORTH:
      return "NORTH";
      break;
    case EAST:
      return "EAST";
      break;
    case SOUTH:
      return "SOUTH";
      break;
    case WEST:
      return "WEST";
      break;

    case NA:
      return "NA";
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
    
    default:
      return "DATA";
  }
}
