const int spring1 = 9;
const int spring2 = 10;
const int spring3 = 11;

void setup() {
  
  pinMode(spring1, OUTPUT);
  pinMode(spring2, OUTPUT);
  pinMode(spring3, OUTPUT);
}

void loop() {
  analogWrite(spring1, 0);
  analogWrite(spring2, 0);
  analogWrite(spring3, 0);
  delay(6000);
  analogWrite(spring1, 255);
  analogWrite(spring2, 0);
  analogWrite(spring3, 0);
  delay(4000);
  analogWrite(spring1, 0);
  analogWrite(spring2, 255);
  analogWrite(spring3, 0);
  delay(4000);
  analogWrite(spring1, 0);
  analogWrite(spring2, 0);
  analogWrite(spring3, 255);
  delay(4000);
  analogWrite(spring1, 0);
  analogWrite(spring2, 0);
  analogWrite(spring3, 0);
  delay(6000);
}
