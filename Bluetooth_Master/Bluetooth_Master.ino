#define ledPin 13
int state = 0;
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {
 if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
 }
 // Controlling the LED
 if (state == '1') {
  digitalWrite(ledPin, HIGH); // LED ON
  state = 0;
 }
 else if (state == '0') {
  digitalWrite(ledPin, LOW); // LED OFF
  state = 0;
 }
}

/**void setup() {
  Serial.begin(9600); // Default communication rate of the Bluetooth module
  pinMode(13, OUTPUT);
  delay(100);
  Serial.println("Hello world");
}
String msg;
void loop() {
 if(Serial.available()){ // Checks whether data is coming from the serial port
    digitalWrite(13, HIGH);
    msg = Serial.readStringUntil('\n'); // Reads the data from the serial port
    if (msg.equals("Hello world")) {
      int prevTime = millis();
      while (prevTime -  millis() < 3000) {
        if (prevTime % 500 < 250) {
          digitalWrite(13, HIGH);
        }
        else {
          digitalWrite(13, LOW);
        }
      }
    }
    else {
      digitalWrite(13, HIGH);
      delay(1000);
      Serial.println("Hello world");
      digitalWrite(13, LOW);
    }
 }
 delay(10);
}**/
