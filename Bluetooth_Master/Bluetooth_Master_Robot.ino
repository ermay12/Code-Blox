
//recieving errors from slave in terms of tile row col
//sending to slave row, col, left, right

#define ledPin 13

int fakeData[] = {0, 0, 12, 16, 0, 1, 13, 13, 1, 0, 10, 5, -1}; //-1 means end of data tx

void setup() {
  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, LOW);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {

  int dataPos = 0;

  //error in form of (row, col); if (-1, -1) then no error
  int error1 = -1;
  int error2 = -1;

  while (fakeData[dataPos] != -1) { //tx data
    Serial.write(fakeData[dataPos]);
    dataPos += 1;
  }
 
  while (Serial.available() == 0) ; //blocking to wait for data to come from slave

  if (Serial.available() > 0) { //error data coming from slave (row)
    error1 = Serial.read();
  }

  if (Serial.available() > 0) { // error data coming from slave (col)
    error2 = Serial.read();
  }

  if ((error1 == -1) && (error2 == -1)) {
    OrangutanLCD::print("Done");
  } else {
    OrangutanLCD::print("Error");
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print("Row: ");
    OrangutanLCD::print(error1);
    OrangutanLCD::print(" Col: ");
    OrangutanLCD::print(error2);  
  }
 
}

/**
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
**/
