void setup() {
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {
 Serial.write('1'); // Sends '1' to the master to turn on LED
 delay(3000);
 Serial.write('0');
 delay(3000);  
}

/**void setup() {
  Serial.begin(9600); // Default communication rate of the Bluetooth module
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}
void loop() {
 if(Serial.available()){ // Checks whether data is coming from the serial port
    digitalWrite(13, HIGH);
    Serial.write(Serial.read());
 }
}**/
