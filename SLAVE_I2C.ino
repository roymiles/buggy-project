#include <Wire.h>

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

enum positionDataState {
  PS_NA = NA, 
  PS_AWAITING_X = AWAITING_X, 
  PS_AWAITING_Y = AWAITING_Y,
  PS_AWAITING_ORIENTATION = AWAITING_ORIENTATION
}; 

/*
 * Used to track whether the buggy is awaiting an x, y or an orienation from the master
 */
positionDataState currentPositionDataState = PS_NA;

// Include the required Wire library for I2C<br>#include <Wire.h>
uint16_t recievedVal = 0;
void setup() {
  Serial.println("Slave setup");
  // Start the I2C Bus as Slave on address 9
  Wire.begin(44); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  recievedVal = Wire.read();    // read one character from the I2C

  /*
   * Awaiting either the x, y or the orientation
   */
  if(currentPositionDataState != NA){
    switch(currentPositionDataState){
      case AWAITING_X:
        Serial.print("Recieved x-coordinate: ");
        Serial.println(commandToData(recievedVal));
        currentPositionDataState = PS_AWAITING_Y;
        break;
      case AWAITING_Y:
        Serial.print("Recieved y-coordinate: ");
        Serial.println(commandToData(recievedVal));
        currentPositionDataState = PS_AWAITING_ORIENTATION;
        break;
      case AWAITING_ORIENTATION:
        Serial.print("Recieved orientation: ");
        Serial.println(commandToString(recievedVal));
        currentPositionDataState = PS_NA;
        break;
    }

    return;
  }
  
  /*
   * Perform based on the received command
   * If the command is not known, then it will be a value that is used dependning on the state
   */
  switch(recievedVal){
    case MASTER_SEND_POSITION:
      Serial.println("Recieved MASTER_SEND_POSITION");
      currentPositionDataState = PS_AWAITING_X;
      break;
  }
}


void loop() {
  delay(100);
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

