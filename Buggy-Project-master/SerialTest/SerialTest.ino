/*
 * The sketch here will make Leonardo exchange
 * data between the USB serial port and SIM808 module.
*/

void setup() {
  Serial.begin(115200); //initialize Serial(i.e. USB port)
  Serial1.begin(115200); //initialize Serial1
}

void loop() {
  //If Serial1 receive data, print out to Serial
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  //If Serial receive data, print out to Serial1
  while (Serial.available()) {
    Serial1.write(Serial.read());
  }
  delay(1);  //delay for a short time to
  // avoid unstable USB communication
}
