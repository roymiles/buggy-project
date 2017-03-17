#include <Wire.h>
// #include "EasyTransferI2C.h"

uint16_t randomNumber;
enum I2C_COMMAND : int // Used for static conversions
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
  
  FORWARD = 16,
  BACKWARDS = 17,
  TURN_LEFT = 18,
  TURN_RIGHT = 19,

  MIN = 0,
  MAX = 19

  /*
   * Anything higher is interpreted as a data value
   */
};

#define SLAVE_ID 44

uint16_t dataToCommand(uint16_t d){ 
  return  d + (MAX+1);
}

uint16_t commandToData(I2C_COMMAND cmd){
  return cmd  - (MAX+1);
}

int val = 0;
void setup() {
  // Start the I2C Bus as Master
  Wire.begin();

  Serial.begin(9600);  // start serial for output
}
void loop() {
  /*
   * Master wants to transmit current buggy position
   */
  Wire.beginTransmission(SLAVE_ID); // transmit to device #44 (0x2c)
                              // device address is specified in datasheet
  Serial.print("Transmitting: ");
  Serial.println(commandToString(I2C_COMMAND::MASTER_SEND_POSITION));                            
  Wire.write(I2C_COMMAND::MASTER_SEND_POSITION); // sends value byte  
  Wire.endTransmission();           // stop transmitting
  delay(500);

  /*
   * Send current x-coordinate
   */
  Wire.beginTransmission(SLAVE_ID);
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
  Wire.beginTransmission(SLAVE_ID);
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
  Wire.beginTransmission(SLAVE_ID);
  Serial.print("Transmitting: ");
  Serial.println(commandToString(I2C_COMMAND::WEST));                            
  Wire.write(I2C_COMMAND::WEST); 
  Wire.endTransmission();
  delay(1000);

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
   * I have literally no idea how this works
   */
  I2C_COMMAND cmd = static_cast<I2C_COMMAND>(recievedVal[0]); //(recievedVal[1] << 8) | recievedVal[0];
//  Serial.print("tmp:" );
//  Serial.println(tmp);
//  Serial.print("forward: ");
//  Serial.println(I2C_COMMAND::FORWARD);
  //I2C_COMMAND cmd = static_cast<I2C_COMMAND>(recievedVal[0] | recievedVal[1] << 8);
  Serial.print("Recieved: ");
  Serial.println(commandToString(cmd));
  delay(500);
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

    case BACKWARDS:
      return "BACKWARDS";
      break;
    case FORWARD:
      return "FORWARDS";
      break;
    case TURN_LEFT:
      return "TURN_LEFT";
      break;
    case TURN_RIGHT:
      return "TURN_RIGHT";
      break;
    
    default:
      return "DATA or UNKNOWN";
  }
}
