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

void loop() {
  digitalWrite(ENABLE_LEFT_WRITE, LOW);
  digitalWrite(LED_GREEN, digitalRead(CLOCK_READ));
  digitalWrite(LED_RED, !digitalRead(CLOCK_READ));
  delay(1000);
  digitalWrite(ENABLE_LEFT_WRITE, HIGH);
  
  digitalWrite(CLOCK_WRITE, LOW);
  digitalWrite(LED_GREEN, digitalRead(DATA_READ));
  digitalWrite(LED_RED, !digitalRead(DATA_READ));
  delay(1000);
  digitalWrite(CLOCK_WRITE, HIGH);
  
  digitalWrite(DATA_WRITE, LOW);
  digitalWrite(LED_GREEN, digitalRead(ENABLE_TOP_READ));
  digitalWrite(LED_RED, !digitalRead(ENABLE_TOP_READ));
  delay(1000);
  digitalWrite(DATA_WRITE, HIGH);
  
  digitalWrite(ENABLE_TOP_WRITE, LOW);
  digitalWrite(LED_GREEN, digitalRead(ENABLE_RIGHT_READ));
  digitalWrite(LED_RED, !digitalRead(ENABLE_RIGHT_READ));
  delay(1000);
  digitalWrite(ENABLE_TOP_WRITE, HIGH);
  
  digitalWrite(ENABLE_RIGHT_WRITE, LOW);
  digitalWrite(LED_GREEN, digitalRead(ENABLE_BOTTOM_READ));
  digitalWrite(LED_RED, !digitalRead(ENABLE_BOTTOM_READ));
  delay(1000);
  digitalWrite(ENABLE_RIGHT_WRITE, HIGH);
  
  digitalWrite(ENABLE_BOTTOM_WRITE, LOW);
  digitalWrite(LED_GREEN, digitalRead(ENABLE_LEFT_READ));
  digitalWrite(LED_RED, !digitalRead(ENABLE_LEFT_READ));
  delay(1000);
  digitalWrite(ENABLE_BOTTOM_WRITE, HIGH);
}
