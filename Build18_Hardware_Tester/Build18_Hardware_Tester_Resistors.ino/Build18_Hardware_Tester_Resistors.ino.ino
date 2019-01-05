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

void clearOutputs(){
  //set all outputs to off
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(CLOCK_WRITE, HIGH);
  digitalWrite(DATA_WRITE, HIGH);
  digitalWrite(ENABLE_TOP_WRITE, HIGH);
  digitalWrite(ENABLE_RIGHT_WRITE, HIGH);
  digitalWrite(ENABLE_BOTTOM_WRITE, HIGH);
  digitalWrite(ENABLE_LEFT_WRITE, HIGH);
}

void setup() {
  // put your setup code here, to run once:

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

  clearOutputs();
}

int cycle = 0;
unsigned int resistance = 0;

void loop() {
  
  clearOutputs();
  switch(cycle){
    case 0:
      resistance = analogRead(RESISTOR_LEFT); 
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(CLOCK_WRITE, !(1&(resistance>>6)));
      digitalWrite(DATA_WRITE, !(1&(resistance>>7)));
      digitalWrite(ENABLE_TOP_WRITE, !(1&(resistance>>8)));
      digitalWrite(ENABLE_RIGHT_WRITE, !(1&(resistance>>9)));
      break;
    case 1:
      resistance = analogRead(RESISTOR_LEFT); 
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(CLOCK_WRITE, !(1&(resistance>>0)));
      digitalWrite(DATA_WRITE, !(1&(resistance>>1)));
      digitalWrite(ENABLE_TOP_WRITE, !(1&(resistance>>2)));
      digitalWrite(ENABLE_RIGHT_WRITE, !(1&(resistance>>3)));
      digitalWrite(ENABLE_BOTTOM_WRITE, !(1&(resistance>>4)));
      digitalWrite(ENABLE_LEFT_WRITE, !(1&(resistance>>5)));
      break;
    case 2:
      resistance = analogRead(RESISTOR_RIGHT); 
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(CLOCK_WRITE, !(1&(resistance>>6)));
      digitalWrite(DATA_WRITE, !(1&(resistance>>7)));
      digitalWrite(ENABLE_TOP_WRITE, !(1&(resistance>>8)));
      digitalWrite(ENABLE_RIGHT_WRITE, !(1&(resistance>>9)));
      break;
      break;
    case 3:
      resistance = analogRead(RESISTOR_RIGHT); 
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(CLOCK_WRITE, !(1&(resistance>>0)));
      digitalWrite(DATA_WRITE, !(1&(resistance>>1)));
      digitalWrite(ENABLE_TOP_WRITE, !(1&(resistance>>2)));
      digitalWrite(ENABLE_RIGHT_WRITE, !(1&(resistance>>3)));
      digitalWrite(ENABLE_BOTTOM_WRITE, !(1&(resistance>>4)));
      digitalWrite(ENABLE_LEFT_WRITE, !(1&(resistance>>5)));
      break;
  }
  cycle++;
  cycle = cycle%4;
  delay(1000);
}
