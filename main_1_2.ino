 #include <NewPing.h>

 #define SONAR_NUM     3
 #define MAX_DISTANCE  200 //max distance in cm
 #define PING_INTERVAL 33 //If processes take longer than 33ms, increase this
 #define SPRING_NUM    3
 
 unsigned long int reg; //for white noise
 
  //---Piezo Buzzer---//
  const int buzzer1 = 8;
  const int buzzer2 = 12;
  const int buzzer3 = 13;

 //---distance sensor---//
 unsigned long pingTimer[SONAR_NUM]; //holds time for next ping
 unsigned int cm[SONAR_NUM]; //ping distance
 uint8_t currentSensor = 0;  //active sensor tracker
 
 NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(4, 7, MAX_DISTANCE), // trigger pin, echo pin, and max ping distance
  NewPing(8, 44, MAX_DISTANCE),
  NewPing(45, 20, MAX_DISTANCE),
 };
 
 //---Spring Heater---//
  class Heater {
    	int springPin;      
    	long HeatTime;     // milliseconds of on-time
    	long CoolTime;    // milliseconds of off-time
     
    	int springState;             		//holds current heat/cool state
    	unsigned long previousMillis;  	// holds last update time
      // Constructor - creates a Heater and initializes the member variables and state
      public:
      Heater(int pin, long on, long off)
      {
    	springPin = pin;
    	pinMode(springPin, OUTPUT);     
    	  
    	HeatTime = on;
    	CoolTime = off;
    	
    	springState = 0; 
    	previousMillis = 0;
      }
      
 
 //Updates//
      void Update()
      {
        // check to see if it's time to change the state of the spring
        unsigned long currentMillis = millis();
         
        if((springState == 255) && (currentMillis - previousMillis >= HeatTime))
        {
        	springState = 0;  // Turn it off
          previousMillis = currentMillis;  // Remember the time
          analogWrite(springPin, springState);  // Update the actual spring
        }
        else if ((springState == 0) && (currentMillis - previousMillis >= CoolTime))
        {
          springState = 255;  // turn it on
          previousMillis = currentMillis;   // Remember the time
          analogWrite(springPin, springState);	  // Update the actual spring
        }
      }
    };
    
    Heater spring1(9, 4000, 25000);
    Heater spring2(10, 4000, 25000);
    Heater spring3(11, 4000, 25000);
    
 void setup() {
  Serial.begin(115200);
  
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(buzzer3, OUTPUT);
  reg = 0x55aa55aaL; //The seed for the bitstream. It can be anything except 0.
  
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;   
}


 void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // Other code that *DOESN'T* analyze ping results can go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  // do something with the ping results.
  if (cm[0] < 40 && cm[0] > 1) {
    
}
