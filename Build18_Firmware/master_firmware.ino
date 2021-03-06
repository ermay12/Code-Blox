#define CLOCK 2
#define DATA 9
#define ENABLE 8
#define BUTTON_PIN 10
#define LED_PIN 11

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

#define DEBOUNCE_DELAY 50

#define CLOCK_PERIOD_MICROS 600

char l_resistors[10][10];
char r_resistors[10][10];
char block_ids[10][10];
char id = 1;
char message_buffer[100];


bool enable_first_tile(unsigned int id){
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, LOW);
  unsigned int result = send_receive(id, COMMAND_GREEN_BLINK);
  pinMode(ENABLE, INPUT_PULLUP);
  if( result == 0x3ff ){
    return true;
  }
  return false;
}

unsigned int send_receive(unsigned int id, unsigned int cmd){
  
  for(int i = 0; i < 5; i++){
    digitalWrite(CLOCK, LOW);
    digitalWrite(DATA, (id >> i) & 1);
    delayMicroseconds(CLOCK_PERIOD_MICROS/2);
    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(CLOCK_PERIOD_MICROS/2);
  }
  for(int i = 0; i < 5; i++){
    digitalWrite(CLOCK, LOW);
    digitalWrite(DATA, (cmd >> i) & 1);
    delayMicroseconds(CLOCK_PERIOD_MICROS/2);
    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(CLOCK_PERIOD_MICROS/2);
  }
  pinMode(DATA, INPUT);
  unsigned int result = 0;
  for(int i = 0; i < 10; i++){
    digitalWrite(CLOCK, LOW);
    delayMicroseconds(CLOCK_PERIOD_MICROS/2);
    digitalWrite(CLOCK, HIGH);
    result = (result << 1) | digitalRead(DATA);
    delayMicroseconds(CLOCK_PERIOD_MICROS/2);
  }
  return result;
}


void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(ENABLE, INPUT_PULLUP);
  digitalWrite(CLOCK, HIGH);
  delayMicroseconds(CLOCK_PERIOD_MICROS/2);

  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      l_resistors[i][j] = 1;
      r_resistors[i][j] = 14;
      block_ids[i][j] = 0;
      message_buffer[i*10 + j] = 0;
    }
  }

  Serial.begin(9600);
  // put your setup code here, to run once:

}

char volt_to_resistance(int v){
  return v*20/(1024);
}

void reset_blocks(){
  for (int i = 0; i < 30; i++){
    send_receive(i, COMMAND_RESET);
  }
  id = 1;
}

void traverse_blocks(int row, int col){
  int my_id = id;
  block_ids[row][col] = my_id;
  int l_volt = send_receive(id, COMMAND_LEFT_RESISTOR);
  int r_volt = send_receive(id, COMMAND_RIGHT_RESISTOR);
  char l_res = volt_to_resistance(l_volt);
  char r_res = volt_to_resistance(r_volt);
  l_resistors[row][col] = l_res;
  r_resistors[row][col] = r_res;

  send_receive(my_id, COMMAND_ENABLE_RIGHT);
  if (send_receive(id+1, COMMAND_CHIRP)){
    id++;
    traverse_blocks(row, col+1);
  }
  send_receive(my_id, COMMAND_ENABLE_BOTTOM);
  if (send_receive(id+1, COMMAND_CHIRP)){
    id++;
    traverse_blocks(row + 1, col);
  }
  send_receive(my_id, COMMAND_ENABLE_LEFT);
  if (send_receive(id+1, COMMAND_CHIRP)){
    id++;
    traverse_blocks(row, col-1);
  }
}

void send_blocks(){
  int index = 0;
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      if (block_ids[i][j]){
        message_buffer[index] = i;
        index++;
        message_buffer[index] = j;
        index++;
        message_buffer[index] = l_resistors[i][j];
        index++;
        message_buffer[index] = r_resistors[i][j];
        index++;
      }
    }
  }
  message_buffer[index] = -1;
  index = 0;
  while (message_buffer[index] != -1) { //tx data
    Serial.write(message_buffer[index]);
    index += 1;
  }
}

void loop() {
  // wait for button press
  int last_debounce = millis();
  int last_state = digitalRead(BUTTON_PIN);
  
  while(true){
    int state = digitalRead(BUTTON_PIN);
    if (state != last_state){
      last_debounce = millis();
    }
    if ((millis() - last_debounce > DEBOUNCE_DELAY) and (state)){
      break;
    }
    else{
      last_state = state;
    }
  }

  digitalWrite(LED_PIN, HIGH);
  reset_blocks();

  if (enable_first_tile(id)){
    traverse_blocks(0,0);
  }
  send_blocks();
  digitalWrite(LED_PIN, LOW);
}
