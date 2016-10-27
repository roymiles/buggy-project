# Buggy-Project

See http://opencv-srf.blogspot.co.uk/2013/05/installing-configuring-opencv-with-vs.html for how to set up opencv with visual studio in a c++ project

## Idea for code structure

We will need to keep our code organised such that we can easily debug and test

    class entry{
      static void main(char[] args){
        // Entry point for the program

        // Example
        communications c = new communications();
        // We can inject the communications object into all dependable classes
        imageProcessing i = new imageProcessing(c);
      }
    }

    // Data structure for any data that needs to be sent to the base station
    class dType{
      char type; // Whether data is an x,y position or gathered from the interrogation
      char[] data;
    }

    class communications{
       // Send data to the base station
       public function send(dType data){}

       // Convert the data to an appropriate format before sending
       private function toJson(dType data){}

       // Check if the buggy is ready to start
       private function isStart(){}
    }

    class imageProcessing{
      const floorToCameraDistance;

      // Get image from the arduino module
      private function getImage(){}

      // Calculate the velocity of the buggy based on the correlation of two adjacent image frames
      private function calculateVelocity(){}
    }

    class movement{
      // Turn the buggy x degrees clockwise
      public function turn(double degrees);
      public function turnLeft(){ this.turn(-90); }
      public function turnRight(){ this.turn(90); }
      public function moveForward(){}
    }

    class pathFinding{
    }

    // etc...



