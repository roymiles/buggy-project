#include <Wire.h>

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
  
  I2C_NORTH = 8,
  I2C_EAST = 9,
  I2C_SOUTH = 10,
  I2C_WEST = 11,
  
  DEFAULT_SLAVE_STATE = 12,
  AWAITING_X = 13,
  AWAITING_Y = 14,
  AWAITING_ORIENTATION = 15,
  
  I2C_FORWARD = 16,
  I2C_BACKWARDS = 17,
  I2C_TURN_LEFT = 18,
  I2C_TURN_RIGHT = 19,

  I2C_MIN = 0,
  I2C_MAX = 19

  /*
   * Anything higher is interpreted as a data value
   */
};

#define SLAVE_ID 44

int dataToCommand(int d){ 
  return  d + (I2C_MAX+1);
}

int commandToData(I2C_COMMAND cmd){
  return cmd  - (I2C_MAX+1);
}

enum positionDataState {
  PS_DEFAULT_SLAVE_STATE = DEFAULT_SLAVE_STATE, 
  PS_AWAITING_X = AWAITING_X, 
  PS_AWAITING_Y = AWAITING_Y,
  PS_AWAITING_ORIENTATION = AWAITING_ORIENTATION
}; 

/*
 * Used to track whether the buggy is awaiting an x, y or an orienation from the master
 */
positionDataState currentPositionDataState = PS_DEFAULT_SLAVE_STATE;

// Include the required Wire library for I2C<br>#include <Wire.h>
uint16_t recievedVal = 0;
void setup() {
  Serial.begin(9600);    // serial / USB port
  Serial.println("Slave setup");
  // Start the I2C Bus as Slave on address 9
  Wire.begin(SLAVE_ID); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);

  // When the master requests data from the slave
  Wire.onRequest(requestEvent); // register event
}

void requestEvent() {
  Serial.print("Transmitting: ");
  Serial.println(commandToString(I2C_COMMAND::MASTER_IDLE));
  Wire.write(I2C_COMMAND::MASTER_IDLE); // respond with message
}

void receiveEvent(int bytes) {
  recievedVal = Wire.read();    // read one character from the I2C
  Serial.println(commandToString(recievedVal));
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


