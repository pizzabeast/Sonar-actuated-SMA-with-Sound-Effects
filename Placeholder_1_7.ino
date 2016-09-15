
//state machine setup
const int S_seriesAOn = 1;
const int S_seriesAWait = 2;
const int S_seriesAOff = 3;
const int S_seriesAOffWait = 4;
const int S_seriesBOn = 5;
const int S_seriesBWait = 6;
const int S_seriesBOff = 7;
const int S_seriesBOffWait = 8;
 

//piezo buzzers set up
const int buzzerA = 5;
const int buzzerB = 6;
int frequency = 1;
unsigned long int reg;

//ultasonic sensor set up
const int trigPin = 7;
const int echoPin = 8;

//Nitinol springs set up
const int springA = 9;
const int springB = 10;


void setup() {
  Serial.begin(9600);

  pinMode(buzzerA, OUTPUT);
  pinMode(buzzerB, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(springA, OUTPUT);
  pinMode(springB, OUTPUT);

  reg = 0x55aa55aaL; //seed for white noise bitstream
}

 
void loop()
{
  
 long duration, inches, cm;
 
  static int state = S_seriesAOn ;
  static unsigned long ts;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW); //sends out ping
    
  duration = pulseIn(echoPin, HIGH); //reads the echo

  cm = microsecondsToCentimeters(duration); //time to distance

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

 if ((cm <= 40) && (cm >= 2)) { //if sensor is triggered, start the state machine
        Serial.print("starting state machine");
        state = S_seriesAOn;
      }
      
  switch (state)
  { 
    case S_seriesAOn:
      digitalWrite( springA, HIGH);
      generateNoise(frequency);
 
      ts = millis();  // Remember the current time
 
      state = S_seriesAWait;  // Move to the next state
 
      break;
 
    case S_seriesAWait:
      // If x seconds have passed, then move on to the next state.
      if (millis() > ts + 12000)
      {
        state = S_seriesAOff;
      }
 
      break;
 
    case S_seriesAOff:
      digitalWrite( springA, LOW);

      ts = millis(); 
 
      state = S_seriesAOffWait;
 
      break;
 
    case S_seriesAOffWait:
 
      if (millis() > ts + 10000)
      {
        state = S_seriesBOn;
      }
 
      break;
 
    case S_seriesBOn:
      digitalWrite( springB, HIGH);
      generateNoise(frequency);
 
      ts = millis();
 
      state = S_seriesBWait;
 
      break;
 
    case S_seriesBWait:

      if (millis() > ts + 12000)
      {
        state = S_seriesBOff;
      }
 
      break;

     case S_seriesBOff:
      digitalWrite( springB, LOW);

      ts = millis();
 
      state = S_seriesBOffWait;
 
      break;
 
    case S_seriesBOffWait:

      if (millis() > ts + 12000)
      {
       state = S_seriesAOn;
      }
 
      break;
 
  } // end of switch
 

 
}
//code for white noise
void generateNoise(int frequency) {
  unsigned long int newr;
  unsigned char lobit;
  unsigned char b31, b29, b25, b24;
   
  b31 = (reg & (1L << 31)) >> 31;
  b29 = (reg & (1L << 29)) >> 29;
  b25 = (reg & (1L << 25)) >> 25;
  b24 = (reg & (1L << 24)) >> 24;

  lobit = b31 ^ b29 ^ b25 ^ b24;
  newr = (reg << 1) | lobit;
  reg = newr;
  
  digitalWrite(buzzerA, reg & 1);
  digitalWrite(buzzerB, reg & 1);
  
  delayMicroseconds(frequency);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
