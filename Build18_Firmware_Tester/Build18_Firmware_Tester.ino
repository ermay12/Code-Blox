
#define CLOCK 11
#define DATA 10
#define ENB_LEFT 12
#define ENB_TOP 9
#define ENB_RIGHT 8
#define ENB_BOTTOM 13

#define CLOCK_LED 7
#define DATA_LED 6
#define ENB_LEFT_LED 2
#define ENB_TOP_LED 5
#define ENB_RIGHT_LED 4
#define ENB_BOTTOM_LED 3


bool top_write = false;
bool right_write = false;
bool bottom_write = false;
bool left_write = false;
bool clock_write = false;
bool data_write = false;


void setup() {

  pinMode(CLOCK, OUTPUT);
  digitalWrite(CLOCK, LOW);

  pinMode(DATA, OUTPUT);
  digitalWrite(DATA, LOW);

  pinMode(ENB_TOP, INPUT_PULLUP);
  pinMode(ENB_RIGHT, INPUT_PULLUP);
  pinMode(ENB_BOTTOM, INPUT_PULLUP);
  pinMode(ENB_LEFT, INPUT_PULLUP);

  pinMode(DATA_LED, OUTPUT);
  pinMode(CLOCK_LED, OUTPUT);
  pinMode(ENB_TOP, OUTPUT);
  pinMode(ENB_RIGHT, OUTPUT);
  pinMode(ENB_BOTTOM, OUTPUT);
  pinMode(ENB_LEFT, OUTPUT);  
  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  digitalWrite(CLOCK_LED, clock_write);
  digitalWrite(DATA_LED, data_write);
  if(top_write){
    digitalWrite(ENB_TOP_LED, LOW);
  }else {
    digitalWrite(ENB_TOP_LED, HIGH);
  }
  if(right_write){
    digitalWrite(ENB_RIGHT_LED, LOW);
  }else {
    digitalWrite(ENB_RIGHT_LED, HIGH);
  }
  if(bottom_write){
    digitalWrite(ENB_BOTTOM_LED, LOW);
  }else {
    digitalWrite(ENB_BOTTOM_LED, HIGH);
  }
  if(left_write){
    digitalWrite(ENB_LEFT_LED, LOW);
  }else {
    digitalWrite(ENB_LEFT_LED, HIGH);
  }
  
  if (Serial.available()) {
    // read the incoming byte:
    int pin = Serial.parseInt();
    String action = Serial.readStringUntil('\n');

    if(pin > 13 || pin < 8){
      return;
    }
    if(action.indexOf("0") >= 0){
      String address = "";
      switch(pin){
        case ENB_TOP:
          top_write = true;
          pinMode(ENB_TOP, OUTPUT);
          digitalWrite(ENB_TOP, LOW);
          address = "enb top";
          break;
        case ENB_RIGHT:
          right_write = true;
          pinMode(ENB_RIGHT, OUTPUT);
          digitalWrite(ENB_RIGHT, LOW);
          address = "enb right";
          break;
        case ENB_BOTTOM:
          bottom_write = true;
          pinMode(ENB_BOTTOM, OUTPUT);
          digitalWrite(ENB_BOTTOM, LOW);
          address = "enb bottom";
          break;
        case ENB_LEFT:
          left_write = true;
          pinMode(ENB_LEFT, OUTPUT);
          digitalWrite(ENB_LEFT, LOW);
          address = "enb left";
          break;
        case CLOCK:
          clock_write = false;
          pinMode(CLOCK, OUTPUT);
          digitalWrite(CLOCK, LOW);
          address = "clock";
        case DATA:
          data_write = false;
          pinMode(DATA, OUTPUT);
          digitalWrite(DATA, LOW); 
          address = "data";
      }
      Serial.println("Wrote a low to " + address); 
    }else if(action.indexOf("1") >= 0){
      //TODO add sting representation of each pin
      //case for clock and data
      //replace resistors with wires
      //figure out why enb's don't write. probably wires tbh but check anyway
      switch(pin){
        case ENB_TOP:
          top_write = false;
          pinMode(ENB_TOP, INPUT_PULLUP);
          break;
        case ENB_RIGHT:
          right_write = false;
          pinMode(ENB_RIGHT, INPUT_PULLUP);
          digitalWrite(ENB_RIGHT, LOW);
          break;
        case ENB_BOTTOM:
          bottom_write = false;
          pinMode(ENB_BOTTOM, INPUT_PULLUP);
          break;
        case ENB_LEFT:
          left_write = false;
          pinMode(ENB_LEFT, INPUT_PULLUP);
          break;
        default:
          pinMode(pin, OUTPUT);
          digitalWrite(pin, HIGH);
      }
      Serial.println("Wrote a high to pin " + String(pin));
    }else{ //assume we want to read the pin
      bool value = false;
      switch(pin){
        case ENB_TOP:
          if(top_write){
            value = false;
          }else{
            value = digitalRead(ENB_TOP);
          }
          break;
        case ENB_RIGHT:
          if(right_write){
            value = false;
          }else{
            value = digitalRead(ENB_RIGHT);
          }
          break;
        case ENB_BOTTOM:
          if(bottom_write){
            value = false;
          }else{
            value = digitalRead(ENB_BOTTOM);
          }
          break;
        case ENB_LEFT:
          if(left_write){
            value = false;
          }else{
            value = digitalRead(ENB_LEFT);
          }
          break;
        default:
          pinMode(pin, INPUT);
          value = digitalRead(pin);
      }
      Serial.println("Pin " + String(pin) + " is a logical " + String(value));
    }
  }

}
