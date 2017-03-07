///**
//    BaseStation.cpp
//    Purpose: Communication Module for the Buggy to talk with the command
//             BaseStation.
//
//    @author J. Hawkins
//    @version 1.0 3/2/2017
//*/
//
//#include "BaseStation.h"
//
//BaseStation::BaseStation()
//{
//    //  Check if a shield is available
//    this->bufferSize = BASE_DEFAULT_BUFFER_SIZE;
//
//    /*if (WiFi.status() == WL_NO_SHIELD) {
//        Serial.println("No shield.");
//        this->wifi_enabled = false;
//    } else this->wifi_enabled = true;*/
//    this->wifi_enabled = true;
//
//    this->ssid = BASE_SSID;
//
//}
//
//BaseStation::~BaseStation()
//{
//    if (WiFi.status() == WL_CONNECTED) {
//       WiFi.disconnect();
//    }
//}
//
//
//int BaseStation::connect() {
//
//    if (this->wifi_enabled) {
//
//        // If already connected, disconnect.
//        if (WiFi.status() == WL_CONNECTED) {
//            WiFi.disconnect();
//        }
//
//        bool exists = false;
//        // Check Network Exists
//        uint8_t numNet = WiFi.scanNetworks();
//        for (int cNet = 0; cNet < numNet; cNet++) {
//
//            if (this->ssid.equals(WiFi.SSID(cNet))) {
//                exists = true;
//                // Break to prevent further processing
//                break;
//            }
//
//		}
//
//		// Initialise connection
//        char* buf = (char*)malloc(this->ssid.length());
//        this->ssid.toCharArray(buf, this->ssid.length());
//
//		WiFi.begin(buf);
//
//		for (int atmpt = 0; atmpt < BASE_CONNECT_ATTEMPTS; atmpt++) {
//
//            delay(BASE_CONNECT_TIME);
//
//            if (WiFi.begin(buf) && wClient.connect(BASE_NETWORK_IP, BASE_NETWORK_PORT)){
//
//                this->wifi_connect = true;
//                free(buf);
//                return BASE_CONNECT_SUCCESS;
//
//            }
//
//		}
//
//        free(buf);
//
//		return BASE_CONNECT_FAILURE;
//
//    } else return EXCEPTION_NO_SHIELD;
//
//}
//
//int BaseStation::listen() {
//
//    if (this->wClient.available()) {
//
//        int read = 0;
//        // Only read in available bytes or fill buffer.
//        while (this->wClient.available()) {
//
//            // Read in current byte
//            this->buffer[read] = this->wClient.read();
//            // Printout byte
//            Serial.print(this->buffer[read]);
//
//            if (++read > this->bufferSize) {
//                // Need to extend buffer.
//            }
//
//        }
//
//
//    } return BASE_NO_DATA;
//
//}
