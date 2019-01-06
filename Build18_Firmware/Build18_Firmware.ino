//pin definitions
#define LED_GREEN 0
#define LED_RED 1
#define CLOCK_READ 2
#define CLOCK_WRITE 4
#define DATA_READ 10
#define DATA_WRITE 3
#define ENABLE_TOP_WRITE 5
#define ENABLE_TOP_READ 11
#define ENABLE_RIGHT_WRITE 6
#define ENABLE_RIGHT_READ 13
#define ENABLE_BOTTOM_WRITE 7
#define ENABLE_BOTTOM_READ 12
#define ENABLE_LEFT_WRITE 8
#define ENABLE_LEFT_READ 9
#define RESISTOR_LEFT A4
#define RESISTOR_RIGHT A5


//period in microseconds
#define CLOCK_PERIOD 8000

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
#define COMMAND_LEFT_RESISTOR 9
#define COMMAND_RIGHT_RESISTOR 10
#define COMMAND_RESET 11
#define COMMAND_CHIRP 12

#define ID_WIDTH 5
#define COMMAND_WIDTH 5
#define DATA_WIDTH 10
#define CYCLE_PERIOD 20

void clearOutputs(){
  digitalWrite(CLOCK_WRITE, HIGH);
  digitalWrite(DATA_WRITE, HIGH);
  digitalWrite(ENABLE_TOP_WRITE, HIGH);
  digitalWrite(ENABLE_RIGHT_WRITE, HIGH);
  digitalWrite(ENABLE_BOTTOM_WRITE, HIGH);
  digitalWrite(ENABLE_LEFT_WRITE, HIGH);
}

void setup() {
  //configure outputs
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(CLOCK_WRITE, OUTPUT);
  pinMode(DATA_WRITE, OUTPUT);
  pinMode(ENABLE_TOP_WRITE, OUTPUT);
  pinMode(ENABLE_RIGHT_WRITE, OUTPUT);
  pinMode(ENABLE_BOTTOM_WRITE, OUTPUT);
  pinMode(ENABLE_LEFT_WRITE, OUTPUT);

  //configure inputs
  pinMode(CLOCK_READ, INPUT);
  pinMode(DATA_READ, INPUT);
  pinMode(ENABLE_TOP_READ, INPUT);
  pinMode(ENABLE_RIGHT_READ, INPUT);
  pinMode(ENABLE_BOTTOM_READ, INPUT);
  pinMode(ENABLE_LEFT_READ, INPUT);

  //set all outputs to off
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  clearOutputs();
}

bool enabled = false;
int cycle_count = 0;
int tile_id = 0;
bool is_addressed = false;
bool is_initialized = false;
int command = 0;

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
  if(clock_reading != digitalRead(CLOCK_READ)){
    clock_reading = !clock_reading;
    clock_time = micros();
  }
  //update the clock state if enough time has passed since the last transition
  if(clock_state != clock_reading && micros() - clock_time > DEBOUNCE_DELAY){
    clock_state = clock_reading;
  }

  if(data_reading != digitalRead(DATA_READ)){
    data_reading = !data_reading;
    data_time = micros();
  }
  if(data_state != data_reading && micros() - data_time > DEBOUNCE_DELAY){
    data_state = data_reading;
  }

  if(enable_top_reading != digitalRead(ENABLE_TOP_READ)){
    enable_top_reading = !enable_top_reading;
    enable_top_time = micros();
  }
  if(enable_top_state != enable_top_reading && micros() - enable_top_time > DEBOUNCE_DELAY){
    enable_top_state = enable_top_reading;
  }
  
  if(enable_right_reading != digitalRead(ENABLE_RIGHT_READ)){
    enable_right_reading = !enable_right_reading;
    enable_right_time = micros();
  }
  if(enable_right_state != enable_right_reading && micros() - enable_right_time > DEBOUNCE_DELAY){
    enable_right_state = enable_right_reading;
  }

  if(enable_bottom_reading != digitalRead(ENABLE_BOTTOM_READ)){
    enable_bottom_reading = !enable_bottom_reading;
    enable_bottom_time = micros();
  }
  if(enable_bottom_state != enable_bottom_reading && micros() - enable_bottom_time > DEBOUNCE_DELAY){
    enable_bottom_state = enable_bottom_reading;
  }

  if(enable_left_reading != digitalRead(ENABLE_LEFT_READ)){
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
    if(!enabled && (enable_top_state || enable_right_state || enable_bottom_state || enable_left_state)){
      enabled = true;
      cycle_count = CYCLE_PERIOD - 1;  
      is_addressed = true;    
    }
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
                break;
            }
          }
        }
      }
    }
    
  }

  prev_clock_state = clock_state;
}





