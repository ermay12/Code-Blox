// Period in microseconds
#define CLOCK_PERIOD 8000

// Pin Definitions
#define CLOCK_READ 0
#define CLOCK_WRITE 1
#define DATA_READ 2
#define DATA_WRITE 3
#define ENABLE_WRITE 4

// Time (microseconds) to wait before a state chance is accepted
#define DEBOUNCE_DELAY CLOCK_PERIOD/8

// Define tile commands
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
#define CHIRP_RESPONSE 1024
#define MAX_BOARD_DIMENSION 59
#define MAX_TILES 30

void clearOutputs(){
  digitalWrite(CLOCK_WRITE, HIGH);
  digitalWrite(DATA_WRITE, HIGH);
  digitalWrite(ENABLE_WRITE, HIGH);
}

void setup() {
  pinMode(CLOCK_WRITE, OUTPUT);
  pinMode(DATA_WRITE, OUTPUT);
  pinMode(CLOCK_READ, INPUT);
  pinMode(DATA_READ, INPUT);
  clearOutputs();
}

bool clock_state = false;
bool data_state = false;
bool prev_clock_state = false;

void debounceInputs(){
  //current pin readings  
  static bool clock_reading = false;
  static bool data_reading = false;
  
  //time after the last state change
  static long clock_time = 0;
  static long data_time = 0;

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
}

int cycle_count = 0;
int curr_row = MAX_TILES;
int curr_col = MAX_TILES;
int curr_tile_id = 0;
bool first_tile_initialized = false;
int board_ids[MAX_BOARD_DIMENSION][MAX_BOARD_DIMENSION];
int board_resistor_encodings[MAX_BOARD_DIMENSION][MAX_BOARD_DIMENSION];

int sendCommand(int tile_id, int command) {
  // TODO
}

void sendToRobot(int board_ids[MAX_BOARD_DIMENSION][MAX_BOARD_DIMENSION],
  int board_resistor_encodings[MAX_BOARD_DIMENSION][MAX_BOARD_DIMENSION]) {
  // TODO  
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


  if (first_tile_initialized == false) {
    if (cycle_count == 0) {
      digitalWrite(ENABLE_WRITE, LOW);
      int response = sendCommand(curr_tile_id, COMMAND_CHIRP);
      if (response != CHIRP_RESPONSE) {
        sendToRobot(board_ids, board_resistor_encodings);
      }
    }
    first_tile_initialized = true;
    board_ids[curr_row][curr_col] = curr_tile_id;
    int left_resistor = sendCommand(curr_tile_id, COMMAND_LEFT_RESISTOR);
    int right_resistor = sendCommand(curr_tile_id, COMMAND_RIGHT_RESISTOR);
    int encoding = getEncoding(left_resistor, right_resistor);
    board_resistor_encodings[curr_row][curr_col] = encoding;
    
  }
  else {
    dfsOnTile(curr_tile_id);
      sendToRobot(board_ids, board_resistor_encodings);
  }
}

int getEncoding(int left_resistor, int right_resistor) {
  // TODO
}

int moves[] = {COMMAND_ENABLE_TOP, COMMAND_ENABLE_RIGHT, COMMAND_ENABLE_BOTTOM, COMMAND_ENABLE_LEFT};

void dfsOnTile(int tile_id) {
  for (int i = 0; i < 4; i++) {
    int curr_move = moves[i];
    sendCommand(tile_id, curr_move);
    int response = sendCommand(curr_tile_id + 1, COMMAND_CHIRP);
    if (response == CHIRP_RESPONSE) {
      curr_tile_id++;
      if (curr_move == COMMAND_ENABLE_TOP) {
        curr_row--;
      }
      else if (curr_move == COMMAND_ENABLE_RIGHT) {
        curr_col++;
      }
      else if (curr_move == COMMAND_ENABLE_BOTTOM) {
        curr_row++;
      }
      else if (curr_move == COMMAND_ENABLE_LEFT) {
        curr_col--;
      }
      board_ids[curr_row][curr_col] = curr_tile_id;

      int left_resistor = sendCommand(curr_tile_id, COMMAND_LEFT_RESISTOR);
      int right_resistor = sendCommand(curr_tile_id, COMMAND_RIGHT_RESISTOR);
      int encoding = getEncoding(left_resistor, right_resistor);
      board_resistor_encodings[curr_row][curr_col] = encoding;
      
      dfsOnTile(curr_tile_id);
    }
  }
}
