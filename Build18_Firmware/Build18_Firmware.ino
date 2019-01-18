//pin definitions
#define LED_GREEN 0
#define LED_RED 1
#define CLOCK 2
#define DATA 4
#define ENABLE_TOP 6
#define ENABLE_RIGHT 8
#define ENABLE_BOTTOM 10
#define ENABLE_LEFT 12
#define RESISTOR_LEFT A1
#define RESISTOR_RIGHT A0


//period in microseconds
#define CLOCK_PERIOD 250

//time (microseconds) to wait before a state change is accepted
#define DEBOUNCE_DELAY CLOCK_PERIOD/16

//define tile commands
#define COMMAND_ENABLE_TOP 1
#define COMMAND_ENABLE_RIGHT 2
#define COMMAND_ENABLE_BOTTOM 3
#define COMMAND_ENABLE_LEFT 4
#define COMMAND_GREEN_OFF 5
#define COMMAND_RED_OFF 6
#define COMMAND_GREEN_ON 7
#define COMMAND_RED_ON 8
#define COMMAND_GREEN_BLINK 9
#define COMMAND_RED_BLINK 10
#define COMMAND_LEFT_RESISTOR 11
#define COMMAND_RIGHT_RESISTOR 12
#define COMMAND_RESET 13
#define COMMAND_CHIRP 14


#define ID_WIDTH 5
#define COMMAND_WIDTH 5
#define DATA_WIDTH 10
#define CYCLE_PERIOD 20
#define BLINK_PERIOD_MS 500


void setup() {
  //configure outputs
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  //configure inputs
  pinMode(CLOCK, INPUT);
  pinMode(DATA, INPUT);
  pinMode(ENABLE_TOP, INPUT_PULLUP);
  pinMode(ENABLE_RIGHT, INPUT_PULLUP);
  pinMode(ENABLE_BOTTOM, INPUT_PULLUP);
  pinMode(ENABLE_LEFT, INPUT_PULLUP);

  //set all outputs to off
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);

  delay(1000);
}

bool enabled = false;
int cycle_count = 0;
int tile_id = 0;
bool is_addressed = false;
bool is_initialized = false;
int command = 0;
bool red_blinking = false;
bool green_blinking = false;

bool clock_state = false;
bool data_state = false;
bool enable_top_state = false;
bool enable_right_state = false;
bool enable_bottom_state = false;
bool enable_left_state = false;

bool prev_clock_state = false;

void debounceInputs(){
  //current pin readings  
  static bool clock_reading = false;
  static bool data_reading = false;
  static bool enable_top_reading = false;
  static bool enable_right_reading = false;
  static bool enable_bottom_reading = false;
  static bool enable_left_reading = false;

  //time after the last state change
  static long clock_time = 0;
  static long data_time = 0;
  static long enable_top_time = 0;
  static long enable_right_time = 0;
  static long enable_bottom_time = 0;
  static long enable_left_time = 0;

  //check for transition and update the transition time
  if(clock_reading != digitalRead(CLOCK)){
    clock_reading = !clock_reading;
    clock_time = micros();
  }
  //update the clock state if enough time has passed since the last transition
  if(clock_state != clock_reading && micros() - clock_time > DEBOUNCE_DELAY){
    clock_state = clock_reading;
  }

  if(data_reading != digitalRead(DATA)){
    data_reading = !data_reading;
    data_time = micros();
  }
  if(data_state != data_reading && micros() - data_time > DEBOUNCE_DELAY){
    data_state = data_reading;
  }

  if(enable_top_reading != digitalRead(ENABLE_TOP)){
    enable_top_reading = !enable_top_reading;
    enable_top_time = micros();
  }
  if(enable_top_state != enable_top_reading && micros() - enable_top_time > DEBOUNCE_DELAY){
    enable_top_state = enable_top_reading;
  }
  
  if(enable_right_reading != digitalRead(ENABLE_RIGHT)){
    enable_right_reading = !enable_right_reading;
    enable_right_time = micros();
  }
  if(enable_right_state != enable_right_reading && micros() - enable_right_time > DEBOUNCE_DELAY){
    enable_right_state = enable_right_reading;
  }

  if(enable_bottom_reading != digitalRead(ENABLE_BOTTOM)){
    enable_bottom_reading = !enable_bottom_reading;
    enable_bottom_time = micros();
  }
  if(enable_bottom_state != enable_bottom_reading && micros() - enable_bottom_time > DEBOUNCE_DELAY){
    enable_bottom_state = enable_bottom_reading;
  }

  if(enable_left_reading != digitalRead(ENABLE_LEFT)){
    enable_left_reading = !enable_left_reading;
    enable_left_time = micros();
  }
  if(enable_left_state != enable_left_reading && micros() - enable_left_time > DEBOUNCE_DELAY){
    enable_left_state = enable_left_reading;
  }
}



void loop() {
  bool rising_clock_edge = false;
  bool falling_clock_edge = false;
  
  debounceInputs();
  
  if(!prev_clock_state && clock_state){
    rising_clock_edge = true;
    cycle_count = (cycle_count + 1) % CYCLE_PERIOD;
  }

  if(prev_clock_state && !clock_state){
    falling_clock_edge = true;
  }

  if(falling_clock_edge){
    pinMode(ENABLE_TOP, INPUT_PULLUP);
    pinMode(ENABLE_RIGHT, INPUT_PULLUP);
    pinMode(ENABLE_BOTTOM, INPUT_PULLUP);
    pinMode(ENABLE_LEFT, INPUT_PULLUP);

    if(enabled){
      if(((cycle_count+1)%CYCLE_PERIOD) < ID_WIDTH + COMMAND_WIDTH ){
        pinMode(DATA, INPUT);
        if(cycle_count == CYCLE_PERIOD - 1){
          switch(command){
            case COMMAND_ENABLE_TOP:
              pinMode(ENABLE_TOP, OUTPUT);
              digitalWrite(ENABLE_TOP, LOW);
              break;
            case COMMAND_ENABLE_RIGHT:
              pinMode(ENABLE_RIGHT, OUTPUT);
              digitalWrite(ENABLE_RIGHT, LOW);
              break;
            case COMMAND_ENABLE_BOTTOM:
              pinMode(ENABLE_BOTTOM, OUTPUT);
              digitalWrite(ENABLE_BOTTOM, LOW);
              break;
            case COMMAND_ENABLE_LEFT:
              pinMode(ENABLE_LEFT, OUTPUT);
              digitalWrite(ENABLE_LEFT, LOW);
              break;
          }
        }  
      } else if(is_addressed){
        pinMode(DATA, OUTPUT);
        switch(command){
          case COMMAND_CHIRP:
            digitalWrite(DATA, HIGH);
            break;
          case COMMAND_LEFT_RESISTOR:
            digitalWrite(DATA,(1&(analogRead(RESISTOR_LEFT)>>(((cycle_count+1)%CYCLE_PERIOD) - ID_WIDTH - COMMAND_WIDTH))));
            break;
          case COMMAND_RIGHT_RESISTOR:
            digitalWrite(DATA,(1&(analogRead(RESISTOR_RIGHT)>>(((cycle_count+1)%CYCLE_PERIOD) - ID_WIDTH - COMMAND_WIDTH))));
            break;
          default:
            digitalWrite(DATA, LOW);
        }
      } else {
        pinMode(DATA, INPUT);
      }
    }
  }

  if(rising_clock_edge){

    

    if(!enabled && (!enable_top_state || !enable_right_state || !enable_bottom_state || !enable_left_state)){
      enabled = true;
      cycle_count = 0;  
      is_addressed = true;    
    }

    if(enabled && !is_initialized){
      if(cycle_count < ID_WIDTH){
        tile_id = tile_id | (data_state << cycle_count);
      }
      if(cycle_count == ID_WIDTH-1){
        is_initialized = true;
      }
    } else if (enabled){
      if(cycle_count == 0){
        is_addressed = true;
        command = 0;
      }
      if(cycle_count < ID_WIDTH){
        if(data_state != ((tile_id >> cycle_count) & 1)){
          is_addressed = false;
        }
      }else{
        if(is_addressed){
          if(cycle_count < ID_WIDTH + COMMAND_WIDTH){
            command = command | (data_state << (cycle_count-ID_WIDTH));
          }else{
            switch(command){
              case COMMAND_CHIRP:
                digitalWrite(DATA, HIGH);
                break;
              case COMMAND_GREEN_OFF:
                digitalWrite(LED_GREEN, LOW);
                green_blinking = false;
                break;
              case COMMAND_RED_OFF:
                digitalWrite(LED_RED, LOW);
                red_blinking = false;
                break;
              case COMMAND_GREEN_ON:
                digitalWrite(LED_GREEN, HIGH);
                green_blinking = false;
                break;
              case COMMAND_RED_ON:
                digitalWrite(LED_RED, HIGH);
                red_blinking = false;
                break;
              case COMMAND_GREEN_BLINK:
                green_blinking = true;
                break;
              case COMMAND_RED_BLINK:
                red_blinking = true;
                break;
              case COMMAND_RESET:
                enabled = false;
                red_blinking = false;
                green_blinking = false;
                cycle_count = 0;
                tile_id = 0;
                command = 0;
                is_addressed = false;
                is_initialized = false;
                digitalWrite(LED_GREEN, HIGH);
                digitalWrite(LED_RED, LOW);
                pinMode(DATA, INPUT);
                break;
            }
          }
        }
      }
    }
    
  }

  if(green_blinking){
    if(millis() % BLINK_PERIOD_MS < BLINK_PERIOD_MS/2){
      digitalWrite(LED_GREEN, HIGH);
    } else {
      digitalWrite(LED_GREEN, LOW);
    }
  }

  if(red_blinking){
    if(millis() % BLINK_PERIOD_MS < BLINK_PERIOD_MS/2){
      digitalWrite(LED_RED, HIGH);
    } else {
      digitalWrite(LED_RED, LOW);
    }
  }

  prev_clock_state = clock_state;
}





