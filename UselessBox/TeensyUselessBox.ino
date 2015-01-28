/* Code for the classic useless box, where switching it on causes a dinger to switch itself off.
  In this case made more complex by a design choice that mandated a folding retraction mechanism
  and a desire to articulate the finger tip
*/


#include <Servo.h>  
Servo lowerServo;  // lift lowers Finger
Servo fingerServo;   // finger tip angle
Servo lidServo;
 
int lidPos = 50;    // variable to store the servo position 
int lowerPos = 15;    // variable to store the servo position 
int fingerPos = 50;    // variable to store the servo position 

int lowerDownPos = 10;  //These are all legacy positions from early versions of the code
int lowerUpPos = 140;
int fingerDownPos =74;
int fingerUpPos = 76;
 int switchRead =0;

 unsigned long ShutDownTimer =0; //tracks how long system has been powered up for
 unsigned long currentRunningTimer =0;  //tracks how long the current motion has been running for
 int cycleCounter =0;  //tracks where in the current cycle we are
 int triggerCount=0; //tracks how many times the switch has been activated.
 int cycleSpeed = 100; //millis for complete cycle
 int cycleType = 0; //negative for closing, positive for opening
 PROGMEM byte servoLookUp[3][50] = {{ //MS Excel generated table for positions to allow more complex motions of the three servos
   10,18,26,34,42,50,58,66,74,82,90,98,106,114,122,130,138,146,154,162,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170
 },{10,10,10,10,10,10,10,10,10,10,10,10,14,18,23,27,31,35,39,44,48,52,56,60,65,69,73,77,81,86,90,94,98,102,107,111,115,119,123,128,132,136,140,146,146,146,146,146,146,146
 },{72,72,72,72,72,72,72,72,72,72,72,72,72,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,74,74,74,78,78,78,78,78,78,78,78,78,78 }};  
  
 
void setup() 
{ 
  lowerServo.attach(21);  
  fingerServo.attach(22);
  lidServo.attach(23);  
  Serial.begin(9600);
    pinMode(13,OUTPUT);
  digitalWrite(13,LOW); //just used as an indicator LED in this aplication
  pinMode(16,OUTPUT);   //drives a Polulo Power switch LC to allow power to remain on when switch in off position
  digitalWrite(16,LOW); //and this is where we latch our power on. Setting high will power down abruptly
  pinMode(17,INPUT);    // monitors switch state
//  pinMode(17, INPUT_PULLUP);
  ShutDownTimer = millis();
} 
 
 
void loop() 
{ 
      if (analogRead(17) <500) {// switch has been turned on
          if (cycleType<1) {cycleType=1; currentRunningTimer=millis();}
          ShutDownTimer = millis();
      } else
      {if (cycleType>0&cycleCounter>0) {cycleType=-1;}}
      MoveServo();    
      if (ShutDownTimer-millis()>10000&cycleCounter==0) //no user input for a time, and we are in stow position
          { digitalWrite(16,HIGH); } //set pin16 HIGH and turn off battery power
      else {digitalWrite(13,(millis()/1000%2));} //blinks pin 13 by way of debug/still alive
      delay(10);
} 
