# Buggy-Project

See http://opencv-srf.blogspot.co.uk/2013/05/installing-configuring-opencv-with-vs.html for how to set up opencv with visual studio in a c++ project

## Idea for code structure

We will need to keep our code organised such that we can easily debug and test
*Each class will be in its own file.*

    class entry{
      static void main(char[] args){
        // Entry point for the program

        // Example
        communications c = new communications();
        // We can inject the communications object into all dependable classes
        imageProcessing i = new imageProcessing(c);
        
        // ...
        while(!isFinished){
            // Program main loop
        }
      }
    }

    // Data structure for any data that needs to be sent to the base station
    class dType{
      char type; // Whether data is an x,y position or gathered from the interrogation
      char[] data;
    }

    class communications{
       // Send data to the base station
       public bool send(dType data){}

       // Convert the data to an appropriate format before sending
       private string toJson(dType data){}

       // Check if the buggy is ready to start
       private bool isStart(){}
    }

    class imageProcessing{
      const floorToCameraDistance;

      // Get image from the arduino module
      private image getImage(){}

      // Calculate the velocity of the buggy based on the correlation of two adjacent image frames
      private double calculateVelocity(){}
    }

    class movement{
      public bool isTurning;
      public bool isMoving;
    
      // Turn the buggy x degrees clockwise
      public void turn(double degrees){
        while(turn < degrees){
            // Keep waiting
        }
      }
      public void turnLeft(){ this.turn(-90); }
      public void turnRight(){ this.turn(90); }
      public void moveForward(){}
      public void stop(){}
    }

    class pathFinding{
    }
    
    class interrogation{
    }
    
    class sensors{
        // Return in mm the distance to an object from the front sensor
        public double readFrontSensor(){}
    }
    
    

    // etc...



