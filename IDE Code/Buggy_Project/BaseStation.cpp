/**
BaseStation.cpp
Purpose: Connect to the maze beacon and base station to provide control
and maze information to the buggy.

@author J. Hawkins, Roy Miles
@version 1.0 12/12/2016
*/

#ifdef WIFI_SHIELD_EXISTS
#include "BaseStation.h"
#include "String.h"
#include <SPI.h>
#include <WiFi.h>

BaseStation::BaseStation()
{

	// Start 'disconnected'
	wifi_connect = false;

	if (WiFi.status() == WL_NO_SHIELD) {

		// Prevent further use of the base station
		wifi_init = false;

	}
	else {

		if (NETWORK_NUMBER == 0) {
			full_ssid = NETWORK_SSID;
		}
		else {
			full_ssid = NETWORK_SSID + '_' + String(NETWORK_NUMBER, DEC);
		}

		wifi_init = true;

	}

}


BaseStation::~BaseStation()
{

	if (wifi_connect) {
		wifiClient.stop();
	}

}

bool BaseStation::mazeBeaconExists()
{

	// Don't bother if WiFi is not setup.
	if (wifi_init) {

		// Find number of available networks
		byte numSSID = WiFi.scanNetworks();
		String cssid;

		// Compare each SSID with expected, return true on match
		for (int cNet = 0; cNet < numSSID; cNet++) {

			cssid = WiFi.SSID(cNet);
			if (cssid.equals(full_ssid))
				return true;

		}

	}

	return false;

}

bool BaseStation::mazeBeaconConnect(int attempts)
{

	// Assign full_ssid to temporary buffer.
	char* buf = (char*)malloc(full_ssid.length());
	full_ssid.toCharArray(buf, full_ssid.length());

	WiFi.begin(buf);

	for (int attempt = 0; attempt < attempts; attempt++) {

		// Wait for connection to establish
		delay(CONNECT_TIME);


		// If connected, break and return true
		if (WiFi.begin(buf) == WL_CONNECTED) {

			if (wifiClient.connect(NETWORK_IP, 80)) {
				wifi_connect = true;
				break;
			}

		}

	}

	// Avoid memory leak and clear temporary buffer
	free(buf);

	return wifi_connect;

}

MazeLayout* BaseStation::getMazeLayout() {

	if (wifi_init && wifi_connect) {

		wifiClient.println("GET / HTTP/1.1");
		wifiClient.println("Host: " + (String)NETWORK_IP);
		wifiClient.println("Connection: close");
		wifiClient.println();

		// Wait for response
		String response = "";
		while (wifiClient.available())
			response = response + wifiClient.read();

		// When disconnected, stop
		if (!wifiClient.connected())
			wifiClient.stop();

		// Check response is valid
		String description = response;

		// Check maze description is valid
		if (MazeLayout::isValid(description)) {
			// Return new MazeLayout object
			return new MazeLayout(description);
		}
		else NULL;

	}
	else return NULL;

}
#endif

