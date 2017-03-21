// ConnectAP.ino

#include "esp8266.h"
#include "SoftwareSerial.h"

#define	ssid		"maze_beacon_1"			// you need to change the ssid and password to your wifi's name and password
#define	password	""

Esp8266 wifi;
bool flag=false;

void setup() {

	delay(2000);				// it will be better to delay 2s to wait esp8266 module OK
	pinMode(13, OUTPUT);

	Serial.begin(9600);	
  while (!Serial) {
   ; // wait for serial port to connect. Needed for Leonardo only
  }				
	wifi.begin(&Serial, &Serial);  //Serial is used to communicate with esp8266 module, mySerial is used to print debug message

	if (wifi.connectAP(ssid, password)) {	  // it will return TRUE if it connect successfully 
    Serial.println("WORKED");
		flag = true;
		wifi.debugPrintln("connected to AP"); // if you don't use a serial to debug, DON NOT use this function 
	} else { 
    Serial.println("NOT WORKING");
		wifi.debugPrintln("connect Fail");								
	}

 // 	if you don't want to use debug, you can write like below
	// Serial1.begin(115200);
	// wifi.begin(&Serial1);
	// if (wifi.connectAP(ssid, password)) {
	// 	flag = true;
	// }
	
}

void loop() {
	if (flag) {
	  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	  delay(500);              // wait for 500ms
	  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	  delay(500);              // wait for 500ms	
	}
}

