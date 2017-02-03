/**
    BaseStation.h
    Purpose: Connect to the maze beacon and base station to provide control
      and maze information to the buggy.

    @author J. Hawkins, Roy Miles
    @version 1.0 12/12/2016
*/

#ifndef BaseStation_h
#define BaseStation_h

#ifdef WIFI_SHIELD_EXISTS

#define BASE_NETWORK_SSID "maze_beacon"
#define BASE_NETWORK_NUMBER 0
// IP address for the web server
#define BASE_NETWORK_IP "10.10.10.10"
// No. attempts to connect
#define CONNECT_TIMEOUT 5
// Allowed time for establishing connection (ms)
#define CONNECT_TIME 5000
// Message Preamble and post-amble
#define PREAMBLE "*#*#*#*#"
#define POSTAMBLE "*#"

// Error Messages
#define MSG_NO_CONNECTION "No Connection"

#include "Arduino.h"
#include "MazeLayout.h"
#include "SPI.h"
#include <WiFi.h>

#pragma once
class BaseStation
{
public:
  BaseStation();
  ~BaseStation();


  // Connect to the BaseStation
  int connect();

  // Can maze beacon be found by the Wifi Shield
  bool mazeBeaconExists();
  // Attempt a connection to the beacon, and return success
  bool mazeBeaconConnect(int attempts);

  // Download maze description
  int getMazeLayout(MazeLayout* layout);

private:

	WiFiClient wifiClient;
	boolean wifi_init, wifi_connect;
	String full_ssid;

};

#endif
#endif

