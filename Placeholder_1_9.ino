#include <NewPing.h>

//ultrasonic sensor set up
#define TRIGGER_PIN   7
#define ECHO_PIN      8 
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

unsigned int pingSpeed = 50; 
unsigned long pingTimer;

//state machine setup
const int S_stopped = 0;
const int S_sensorTrigger = 1;
const int S_seriesAOn = 2;
const int S_seriesAWait = 3;
const int S_seriesAOff = 4;
const int S_seriesAOffWait = 5;
const int S_seriesBOn = 6;
const int S_seriesBWait = 7;
const int S_seriesBOff = 8;
const int S_seriesBOffWait = 9;
 
//piezo buzzers set up
const int buzzerA = 5;
const int buzzerB = 6;
unsigned long lastClick;


//Nitinol springs set up
const int springA = 9;
const int springB = 10;
int aState = LOW;
int bState = LOW;


void setup() {
  Serial.begin(9600);
  
  pinMode(buzzerA, OUTPUT);
  pinMode(buzzerB, OUTPUT);

  pinMode(springA, OUTPUT);
  pinMode(springB, OUTPUT);

  lastClick = micros();  
  pingTimer = millis(); // Start now.
}

//white noise setup//
#define LFSR_INIT  0x55aa55aaL
#define LFSR_MASK  ((unsigned long)( 1UL<<31 | 1UL <<29 | 1UL <<25 | 1UL <<24  ))

unsigned int generateNoise(){
  static unsigned long int lfsr = LFSR_INIT;
  if(lfsr & 1) { lfsr =  (lfsr >>1) ^ LFSR_MASK ; return(1);}
   else { lfsr >>= 1; return(0);}
}


void loop() {

   if ((aState == HIGH) || (bState == HIGH)) {
     if ((micros() - lastClick) > 40 ) { // Changing this value changes the frequency.
       lastClick = micros();
       digitalWrite (buzzerA, generateNoise());
       digitalWrite(buzzerB, generateNoise());
     }
   }
  
  if (millis() >= pingTimer) {
    pingTimer += pingSpeed;    
    sonar.ping_timer(echoCheck);
  }
}

void echoCheck() { 
  long cm;
  cm = (sonar.ping_result / US_ROUNDTRIP_CM);
  static int state = S_stopped ;
  static unsigned long ts;

   
  if (sonar.check_timer()) { 
    Serial.print("Ping: ");
    Serial.print(cm);
    Serial.println("cm");


    
  switch (state)
  { 
    case S_stopped:

      state = S_sensorTrigger;

      break;

    case S_sensorTrigger:
    
      if ((cm <= 40) && (cm >= 2)) {
        Serial.print("starting state machine");
        state = S_seriesAOn;
      }
      else {
        state = S_stopped;
      }

      break;
    
    case S_seriesAOn:
      Serial.print("SpringA heating");
      aState = HIGH;
      digitalWrite( springA, HIGH);
 
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
      Serial.print("SpringA cooling");
      aState = LOW;
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
      Serial.print("SpringB heating");
      bState = HIGH;
      digitalWrite( springB, HIGH);
      
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
      Serial.print("SpringB cooling");
      bState = LOW;
      digitalWrite( springB, LOW);

      ts = millis();
 
      state = S_seriesBOffWait;
 
      break;
 
    case S_seriesBOffWait:

      if (millis() > ts + 12000)
      {
       Serial.print("restart");
       state = S_stopped;
      }
 
      break;
 
  } // end of switch
 
} // end of loop

  }
  
  

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

