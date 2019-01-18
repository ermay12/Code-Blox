//recieving errors from slave in terms of tile row col
//sending to slave row, col, left, right

#define ledPin 13

int fakeData[] = {0, 0, 14, 16, 0, 1, 1, 12, 1, 1, 14, 15, 1, 2, 12, 16, 1, 3, 16, 16, 1, 4, 1, 12, 2, 2, 16, 13, 2, 3, 5, 0, 3, 2, 13, 16, 3, 3, 1, 0, 4, 1, 15, 14, 5, 2, 16, 13, 5, 3, 1, 0, 6, 2, 13, 16, 6, 3, 5, 0}; //-1 means end of data tx

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

  while(true);
 
}
