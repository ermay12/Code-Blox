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
#define CLOCK_PERIOD 500

//time (microseconds) to wait before a state change is accepted
#define DEBOUNCE_DELAY CLOCK_PERIOD/8

//define tile commands
#define COMMAND_ENABLE_TOP 1
#define COMMAND_ENABLE_RIGHT 2
#define COMMAND_ENABLE_BOTTOM 3
#define COMMAND_ENABLE_LEFT 4
#define COMMAND_GREEN_OFF 5
#define COMMAND_RED_OFF 6
#define COMMAND_GREEN_ON 7
#define COMMAND_RED_ON 8
#define COMMAND_GREEN_FLASH 9
#define COMMAND_RED_FLASH 10
#define COMMAND_LEFT_RESISTOR 11
#define COMMAND_RIGHT_RESISTOR 12
#define COMMAND_RESET 13
#define COMMAND_CHIRP 14

#define ID_WIDTH 5
#define COMMAND_WIDTH 5
#define DATA_WIDTH 10
#define CYCLE_PERIOD 20

//does not clear led's
void clearOutputs(){
  digitalWrite(CLOCK, HIGH);
  digitalWrite(DATA, HIGH);
  digitalWrite(ENABLE_TOP, HIGH);
  digitalWrite(ENABLE_RIGHT, HIGH);
  digitalWrite(ENABLE_BOTTOM, HIGH);
  digitalWrite(ENABLE_LEFT, HIGH);
}

void setup() {
  //configure outputs
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(CLOCK, INPUT);
  pinMode(DATA, INPUT);
  pinMode(ENABLE_TOP, INPUT);
  pinMode(ENABLE_RIGHT, INPUT);
  pinMode(ENABLE_BOTTOM, INPUT);
  pinMode(ENABLE_LEFT, INPUT);

  //set all outputs to off
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
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

bool enable_top_state = false;
bool enable_right_state = false;
bool enable_bottom_state = false;
bool enable_left_state = false;

bool prev_clock_state = false;

void debounceClock(){
  //current pin readings  
  static bool clock_reading = false;

  //time after the last state change
  static long clock_time = 0;

  //check for transition and update the transition time
  if(clock_reading != digitalRead(CLOCK_READ)){
    clock_reading = !clock_reading;
    clock_time = micros();
  }
  //update the clock state if enough time has passed since the last transition
  if(clock_state != clock_reading && micros() - clock_time > DEBOUNCE_DELAY){
    clock_state = clock_reading;
  }

}

bool readEnablePins(bool rising_clock_edge, bool falling_clock_edge){
  if(rising_clock_edge){
    pinMode(ENABLE_TOP, INPUT);
    pinMode(ENABLE_RIGHT, INPUT);
    if(digitalRead(ENABLE_TOP) == HIGH|| digitalRead(ENABLE_RIGHT) == HIGH){
      
    }
  } else if (falling_clock_edge){
    //TODO
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

  if(!enabled && readEnablePins(rising_clock_edge, falling_clock_edge)){
    enabled = true;
    cycle_count = cycle_count - 1;
    is_addressed = true;
  }


  if(rising_clock_edge){
    clearOutputs();

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
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        if(is_addressed){
          digitalWrite(LED_GREEN, HIGH);
          digitalWrite(LED_RED, LOW);
          if(cycle_count < ID_WIDTH + COMMAND_WIDTH){
            command = command | (data_state << (cycle_count-ID_WIDTH));
          }else{
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, HIGH);
            switch(command){
              case COMMAND_ENABLE_TOP:
                if(cycle_count == CYCLE_PERIOD - 1){
                  digitalWrite(ENABLE_TOP_WRITE, LOW);
                }
                break;
              case COMMAND_ENABLE_RIGHT:
                if(cycle_count == CYCLE_PERIOD - 1){
                  digitalWrite(ENABLE_RIGHT_WRITE, LOW);
                }
                break;
              case COMMAND_ENABLE_BOTTOM:
                if(cycle_count == CYCLE_PERIOD - 1){
                  digitalWrite(ENABLE_BOTTOM_WRITE, LOW);
                }
                break;
              case COMMAND_ENABLE_LEFT:
                if(cycle_count == CYCLE_PERIOD - 1){
                  digitalWrite(ENABLE_LEFT_WRITE, LOW);
                }
                break;
              case COMMAND_GREEN_OFF:
                digitalWrite(LED_GREEN, LOW);
                is_addressed = false;
                break;
              case COMMAND_RED_OFF:
                digitalWrite(LED_RED, LOW);
                is_addressed = false;
                break;
              case COMMAND_GREEN_ON:
                digitalWrite(LED_GREEN, HIGH);
                is_addressed = false;
                break;
              case COMMAND_RED_ON:
                digitalWrite(LED_RED, HIGH);
                is_addressed = false;
                break;
              case COMMAND_LEFT_RESISTOR:
                digitalWrite(LED_GREEN, HIGH);
                digitalWrite(LED_RED, LOW);
                digitalWrite(DATA_WRITE,!(1&(analogRead(RESISTOR_LEFT)>>(cycle_count - ID_WIDTH - COMMAND_WIDTH))));
                break;
              case COMMAND_RIGHT_RESISTOR:
                digitalWrite(LED_GREEN, HIGH);
                digitalWrite(LED_RED, LOW);
                digitalWrite(DATA_WRITE,!(1&(analogRead(RESISTOR_RIGHT)>>(cycle_count - ID_WIDTH - COMMAND_WIDTH))));
                break;
              case COMMAND_RESET:
                enabled = false;
                cycle_count = 0;
                tile_id = 0;
                command = 0;
                is_addressed = false;
                is_initialized = false;
                digitalWrite(LED_GREEN, LOW);
                digitalWrite(LED_RED, LOW);
                clearOutputs();
                break;
              case COMMAND_CHIRP:
                digitalWrite(LED_GREEN, HIGH);
                digitalWrite(LED_RED, LOW);
                digitalWrite(DATA_WRITE, LOW);
                is_addressed = false;
                break;
            }
          }
        }
      }
    }
    
  }

  prev_clock_state = clock_state;
}





