
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

bool clock_input = false;
bool data_input = false;

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
  pinMode(ENB_TOP_LED, OUTPUT);
  pinMode(ENB_RIGHT_LED, OUTPUT);
  pinMode(ENB_BOTTOM_LED, OUTPUT);
  pinMode(ENB_LEFT_LED, OUTPUT);  

  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  if(!clock_input){
    digitalWrite(CLOCK_LED, clock_write);
  }
  if(!data_input){
    digitalWrite(DATA_LED, data_write);
  }else{
    digitalWrite(DATA_LED, digitalRead(DATA));
  }
  if(top_write){
    digitalWrite(ENB_TOP_LED, LOW);
  }else {
    digitalWrite(ENB_TOP_LED, digitalRead(ENB_TOP));
  }
  if(right_write){
    digitalWrite(ENB_RIGHT_LED, LOW);
  }else {
    digitalWrite(ENB_RIGHT_LED, digitalRead(ENB_RIGHT));
  }
  if(bottom_write){
    digitalWrite(ENB_BOTTOM_LED, LOW);
  }else {
    digitalWrite(ENB_BOTTOM_LED, digitalRead(ENB_BOTTOM));
  }
  if(left_write){
    digitalWrite(ENB_LEFT_LED, LOW);
  }else {
    digitalWrite(ENB_LEFT_LED, digitalRead(ENB_LEFT));
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
          clock_input = false;
          clock_write = false;
          pinMode(CLOCK, OUTPUT);
          digitalWrite(CLOCK, LOW);
          address = "clock";
          break;
        case DATA:
          data_input = false;
          data_write = false;
          pinMode(DATA, OUTPUT);
          digitalWrite(DATA, LOW); 
          address = "data";
          break;
      }
      Serial.println("Wrote a low to " + address); 
    }else if(action.indexOf("1") >= 0){
      String address = "";
      switch(pin){
        case ENB_TOP:
          address = "enb top";
          top_write = false;
          pinMode(ENB_TOP, INPUT_PULLUP);
          break;
        case ENB_RIGHT:
          address = "enb right";
          right_write = false;
          pinMode(ENB_RIGHT, INPUT_PULLUP);
          digitalWrite(ENB_RIGHT, LOW);
          break;
        case ENB_BOTTOM:
          address = "enb bottom";
          bottom_write = false;
          pinMode(ENB_BOTTOM, INPUT_PULLUP);
          break;
        case ENB_LEFT:
          address = "enb left";
          left_write = false;
          pinMode(ENB_LEFT, INPUT_PULLUP);
          break;
        case CLOCK:
          clock_input = false;
          clock_write = true;
          pinMode(CLOCK, OUTPUT);
          digitalWrite(CLOCK, HIGH);
          address = "clock";
          break;
        case DATA:
          data_input = false;
          data_write = true;
          pinMode(DATA, OUTPUT);
          digitalWrite(DATA, HIGH);
          address = "data";
          break;
      }
      Serial.println("Wrote a high to pin " + address);
    }else{ //assume we want to read the pin
      bool value = false;
      String address = "";
      switch(pin){
        case ENB_TOP:
          address = "enb top";
          if(top_write){
            value = false;
          }else{
            value = digitalRead(ENB_TOP);
          }
          break;
        case ENB_RIGHT:
          address = "enb right";
          if(right_write){
            value = false;
          }else{
            value = digitalRead(ENB_RIGHT);
          }
          break;
        case ENB_BOTTOM:
          address = "enb bottom";
          if(bottom_write){
            value = false;
          }else{
            value = digitalRead(ENB_BOTTOM);
          }
          break;
        case ENB_LEFT:
          address = "enb left";
          if(left_write){
            value = false;
          }else{
            value = digitalRead(ENB_LEFT);
          }
          break;
        case CLOCK:
          clock_input = true;
          address = "clock";
          pinMode(CLOCK, INPUT);
          delay(1);
          value = digitalRead(CLOCK);
          break;
        case DATA:
          data_input = true;
          address = "data";
          pinMode(DATA, INPUT);
          delay(1);
          value = digitalRead(DATA);
          break;
      }
      Serial.println(address + " is a logical " + String(value));
    }
  }

}
