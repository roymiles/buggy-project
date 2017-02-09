///**
//    BaseStation.h
//    Purpose: Communication Module for the Buggy to talk with the command
//             BaseStation.
//
//    @author J. Hawkins
//    @version 1.0 3/2/2017
//*/
//
//#ifndef BASESTATION_H
//#define BASESTATION_H
//
//#include "Communication.h"
//#include "ESP8266/esp8266.h"
//#include <SPI.h>
//#include <WiFi.h>
//
//// Macro defintions
//#define BASE_SSID "groupd_basestation"
//#define BASE_NETWORK_IP "127.0.0.1"
//#define BASE_NETWORK_PORT 5000
//#define BASE_CONNECT_ATTEMPTS 5
//#define BASE_CONNECT_TIME 1000
//#define BASE_DEFAULT_BUFFER_SIZE 64
//
//#define BASE_CONNECT_SUCCESS 0xF0
//#define BASE_CONNECT_FAILURE 0x0F
//
//
//// Listen responses
//#define BASE_NO_DATA 0x80
//#define BASE_NULL_RESPONSE 0x81
//#define BASE_TRIAL_STARTED 0x82
//
//// Exception Definitions
//#define EXCEPTION_NO_SHIELD 0x10
//
//
//class BaseStation
//{
//    public:
//        BaseStation();
//        ~BaseStation();
//
//        //  Connect to the BaseStation, ideally during setup
//        int connect();
//        //  Listen command to be called during main loop
//        int listen();
//
//
//    protected:
//    private:
//
//        Esp8266 wClient;
//        String ssid;
//
//        char* buffer;
//
//        int bufferSize;
//
//        bool wifi_enabled;
//        bool wifi_connect;
//};
//
//#endif // BASESTATION_H
