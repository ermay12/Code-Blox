void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  if (Serial.available()) {
    // read the incoming byte:
    int pin = Serial.parseInt();
    String action = Serial.readStringUntil('\n');

    if(pin > 13 || pin < 0){
      return;
    }
    
    if(action.indexOf("0") >= 0){
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
      Serial.println("Wrote a low to pin " + String(pin)); 
    }else if(action.indexOf("1") >= 0){
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
      Serial.println("Wrote a high to pin " + String(pin));
    }else{ //assume we want to read the pin
      pinMode(pin, INPUT);
      Serial.println("Pin " + String(pin) + " is a logical " + String(digitalRead(pin)));
    }
  }

}
