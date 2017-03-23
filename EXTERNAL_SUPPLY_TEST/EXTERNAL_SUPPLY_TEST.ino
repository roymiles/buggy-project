void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps:
  
  // put your setup code here, to run once:
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
     Serial.println(F("-------------------------"));
     
    digitalWrite(A3, LOW);
    digitalWrite(A4, LOW);
    digitalWrite(A5, LOW);

    delay(100);
     
    digitalWrite(A3, HIGH);
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH); 

    delay(100);
}
