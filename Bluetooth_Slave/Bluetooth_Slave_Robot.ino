//receiving 2 2d arrays from master (lRes and rRes)

resistance_t lRes[10][10];
resistance_t rRes[10][10];

void setup() {
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {
  
  while (Serial.available() == 0) ; //blocking to wait for data to come from master

  int dataIndex = 0; //0: row, 1: col, 2: left, 3: right
  int row = -1;
  int col = -1;
  
  while (Serial.available() > 0) {
    data = Serial.read();
    if (dataIndex == 0) {
      row = data;
    } else if (dataIndex == 1) {
      col = data;
    } else if (dataIndex == 2) {
      lRes[row][col] = data;
    } else {
      rRes[row][col] = data;
    }
    dataIndex += 1;
    dataIndex %= 4;
  }

  bool success = runCode(10, 10, 0, 0);

  if (success) {
    Serial.write(-1);
    Serial.write(-1);
  } else {
    Serial.write(errorRow);
    Serial.write(errorCol);
  }
  
}

/**
void setup() {
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {
 Serial.write('1'); // Sends '1' to the master to turn on LED
 delay(3000);
 Serial.write('0');
 delay(3000);  
}
**/
