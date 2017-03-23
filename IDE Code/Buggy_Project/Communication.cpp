/**
    Communication.cpp
    Purpose: This class is used to communicate to the SLAVE device in
    order to transmit and recieve data from the basestation

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#include "Communication.h"

Point *Communication::currentCoordinates = new Point(0, 0); // Starting position
int Communication::recievedVal = -1;
I2C_COMMAND Communication::recievedCommand = I2C_DO_NOTHING; // Recieved from the basestation
  
char Communication::dataBuffer[4] = {1, 3, 3, 7};
orientation Communication::currentOrientation = NORTH;
I2C_COMMAND Communication::curState = SLAVE_IDLE;

Communication::Communication()
{
  // Start the I2C Bus as Slave on address 44
  Wire.begin(SLAVE_ID); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(Communication::receiveEvent);
  // When the master requests data from the slave
  Wire.onRequest(Communication::requestEvent); // register event
}

/**
 * Change the current orientation of the buggy. This
 * function needs to be static because it is called
 * by the static interrupt routines
 * @param ornt, the new orientation of the buggy
 */
static void Communication::setCurrentOrientation(orientation ornt){
  //Serial.println("1");
  Communication::currentOrientation = ornt;
  //Serial.println("2");
  Communication::dataBuffer[2] = (char)(ornt & 0xFF); 
  //Serial.println("3");
}

/**
 * Change the current state of the buggy. This
 * function needs to be static because it is called
 * by the static interrupt routines
 * @param state, the new state of the buggy
 */
static void Communication::setCurState(I2C_COMMAND state){
  Communication::curState = state;
  Communication::dataBuffer[3] = (char)(state & 0xFF);
}

int Communication::dataToCommand(int d){
  return  d + (I2C_MAX+1);
}

int Communication::commandToData(I2C_COMMAND cmd){
  return cmd  - (I2C_MAX+1);
} 

/** 
 * Master is requesting data from slave (buggy) 
 * The data is constantly updated in a buffer so to reduce the size
 * of this interrupt routine. The data buffer encapsulates 4 pieces
 * of information: the x, y, orientation and state of the buggy
 */
void Communication::requestEvent() {
  // Send the x, y, orientation and current state (in that index order)
  Wire.write(Communication::dataBuffer, 4);
}

/** 
 * Master has sent data to the slave (buggy) 
 * This data will be encoded as I2C_COMMANDs 
 * See Communication.h for the decleration of
 * this type
 */
void Communication::receiveEvent(int bytes) {
  Communication::recievedVal = Wire.read(); // read one character from the I2C
  Communication::recievedCommand = static_cast<I2C_COMMAND>(Communication::recievedVal);
  Serial.print(F("R: "));
  Serial.print(Communication::recievedCommand);
  Serial.print(F(", "));
  Serial.print(F("S: "));
  Serial.println(Communication::curState);
}

uint32_t Communication::convertDataToI2C(int x, int y, int orientation, I2C_COMMAND state) {

  uint32_t temp = 0;
  temp |= (x & 0xFF) << 24;
  temp |= (y & 0xFF) << 16;
  temp |= (orientation & 0xFF) << 8;
  temp |= (state & 0xFF);

  return temp;

}

void Communication::convertI2CToData(int data, int *x, int *y, int *orientation, I2C_COMMAND *state) {

  *x = (int)((data >> 24) & 0xFF);
  *y = (int)((data >> 16) & 0xFF);
  *orientation = (int)((data >> 8) & 0xFF);
  *state = (I2C_COMMAND)(data & 0xFF);

}

String Communication::commandToString(I2C_COMMAND cmd){
  switch(cmd){
     case SLAVE_IDLE:
      return "SLAVE_IDLE";
      break;
     case SLAVE_MOVING:
      return "SLAVE_MOVING";
      break;
     case SLAVE_WIGGLING:
      return "SLAVE_WIGGLING";
      break;
     case SLAVE_FINISHED:
      return "SLAVE_FINISHED";
      break;   
     case SLAVE_STOPPED:
      return "SLAVE_STOPPED";
      break;  
     case SLAVE_DOCKED:
      return "SLAVE_DOCKED";
      break;
     case SLAVE_DOCKING:
      return "SLAVE_DOCKING";
  
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
    case I2C_VICTORY:
      return "I2C_VICTORY";
      break;
    case I2C_DO_NOTHING:
      return "I2C_DO_NOTHING";
      break;
    case I2C_AT_TARGET:
      return "I2C_AT_TARGET";
      break;
    
    default:
      return "DATA or UNKNOWN";
  } 
}
