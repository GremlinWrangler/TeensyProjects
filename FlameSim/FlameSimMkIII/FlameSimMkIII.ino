// Fire simulation using 8 streams of 12 neo pixels each
// running on a Teensy 3 via the 8 channel OCTOWS811
// Library from PJRC
// Default code takes serial messages from an Xbee
// and parses out the analogue value to use as a 
// remote intensity control
// Battery monitoring will disable display if 
// batMonPin (17) is low

//Fire effect inspired by the Teensy fire example
// but extensivly modified to produce fast rising bursts
// of white heat against a red flickering glow
// intention was to support fire training where existing process
// was for people to wave extuingishers in the direction of
// the fire and then walk away to ask an umpire if the
// fire was out. In this case they can call it as they see it

// One unforseen issue is that in thick smoke an IR camera is
// used and of course LEDs have no presence at all in IR

// Maybe in the MkV version..

#include <OctoWS2811.h>

// The display size and color to use
const unsigned int width = 12;
const unsigned int height = 8;

const unsigned int batMonPin = 17;
const unsigned int statusLED = 10;

// These parameters control the fire appearance
unsigned int heat = width / 5;
unsigned int focus = 20;
unsigned int cool = 500; //low numbers cause minimal reduction, 500 causes rapid cooling going up display
unsigned int flickerSpeed =100;
unsigned int stageOneGain =0;  //at high intensities two seperate noise fields feed heat into the simulation
unsigned int stageTwoGain =0;
float stageTwoOffset = 0;  //noise inputs are sinewave -1<x<1 values, this drives how much the second stage is >1

unsigned int analogueAvBuffer =0; //used for the highly noisey battery monitor
unsigned int lowBatLockout =0; //if set to one disables display until reset.

unsigned int heartBeat =0;  // these two control the indicator LED in it's
unsigned int heartBeatDirection =0; //slow heart beat or fast 'I'm flat' indications

int intensity = 5; //defines how hot our fire is burning 0-1024 
                   // 0 =0 no fire
                   // 100 flickering
                   // 500 red flames
                   // 700 yellow flames
                    // if remote is operating will be updated each time it transmits
// Arrays for fire animation
int canvas[width*height];
byte serialBuffer[14]; //captures Xbee data to allow pattern matching

#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
const unsigned int fireColor[100] = { //100 point lookup table from dim red to blended yellows
RGB(0,0,0),
RGB(2,0,0),
RGB(4,0,0),
RGB(6,0,0),
RGB(9,0,0),
RGB(11,0,0),
RGB(14,0,0),
RGB(16,0,0),
RGB(19,0,0),
RGB(22,0,0),
RGB(25,0,0),
RGB(28,0,0),
RGB(31,0,0),
RGB(35,0,0),
RGB(38,0,0),
RGB(41,0,0),
RGB(45,0,0),
RGB(48,0,0),
RGB(52,0,0),
RGB(56,0,0),
RGB(59,0,0),
RGB(64,0,0),
RGB(68,0,0),
RGB(72,0,0),
RGB(76,0,0),
RGB(79,0,0),
RGB(83,0,0),
RGB(88,0,0),
RGB(92,0,0),
RGB(97,0,0),
RGB(101,0,0),
RGB(105,0,0),
RGB(109,0,0),
RGB(114,0,0),
RGB(118,0,0),
RGB(122,0,0),
RGB(127,0,0),
RGB(131,0,0),
RGB(136,0,0),
RGB(140,0,0),
RGB(145,0,0),
RGB(149,0,0),
RGB(154,0,0),
RGB(159,0,0),
RGB(163,0,0),
RGB(168,0,0),
RGB(174,0,0),
RGB(179,0,0),
RGB(184,0,0),
RGB(188,0,0),
RGB(192,0,0),
RGB(194,2,0),
RGB(197,4,0),
RGB(199,6,0),
RGB(202,8,0),
RGB(204,11,0),
RGB(207,13,0),
RGB(209,16,0),
RGB(212,19,0),
RGB(214,22,0),
RGB(217,25,0),
RGB(219,28,0),
RGB(222,31,0),
RGB(224,35,0),
RGB(227,38,0),
RGB(229,41,0),
RGB(232,45,0),
RGB(234,48,0),
RGB(237,52,0),
RGB(240,56,0),
RGB(242,59,0),
RGB(246,64,0),
RGB(249,68,0),
RGB(251,72,0),
RGB(254,76,0),
RGB(255,80,0),
RGB(255,86,1),
RGB(255,93,2),
RGB(255,99,2),
RGB(255,106,3),
RGB(255,113,4),
RGB(255,119,5),
RGB(255,126,6),
RGB(255,132,8),
RGB(255,139,9),
RGB(255,146,10),
RGB(255,153,11),
RGB(255,161,12),
RGB(255,168,14),
RGB(255,175,15),
RGB(255,182,16),
RGB(255,190,18),
RGB(255,197,19),
RGB(255,204,21),
RGB(255,212,22),
RGB(255,219,24),
RGB(255,228,26),
RGB(255,236,28),
RGB(255,243,29),
RGB(255,251,31)
};


#define LED_LAYOUT = 0;

// OctoWS2811 objects
const int ledsPerPin = width * height / 8;
DMAMEM int displayMemory[ledsPerPin*6];
int drawingMemory[ledsPerPin*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerPin, displayMemory, drawingMemory, config);

// assorted state machine counters
int cycleCounter = 0; //heartbeat delay counter
int tick = 0; //counter that feeds our noise function to produce flowing heat into the simulation
int battMonReadCounter=0; //tracks how many voltage reads we have done.

// Run setup once
void setup() {
  // turn on the display
  leds.begin();
  pinMode(13,OUTPUT);
    pinMode(statusLED,OUTPUT);
    pinMode(batMonPin ,INPUT);
    

  Serial1.begin(9600);
   for (int y=0; y <height-1; y++) {  // loops to load up a graduated arry to confirm LED operation on power up
    for (int x=0; x < width-1; x++) {
        canvas[(y) * width + x]= (x*6+y*6)*256;
        if (y%2==0) leds.setPixel(xy(x, y), 5); else leds.setPixel(xy(11-x, y), 5);
       // leds.show();
    }} 
   // colorWipe(0x009f00, 50);
}


// A simple xy() function to turn display matrix coordinates
// into the index numbers OctoWS2811 requires.  If your LEDs
// are arranged differently, edit this code...
unsigned int xy(unsigned int x, unsigned int y) {
  if ((y & 1) == 0) {
    // even numbered rows (0, 2, 4...) are left to right
    return y * width + x;
  } else {
    // odd numbered rows (1, 3, 5...) are right to left
    return y * width + width - 1 - x;
  }
}

elapsedMillis msec; //Macro to track msec from a time

void checkSerial() //pulls data in from the Xbee and parses it for values.
{

 byte incomingByte;
  	if (Serial1.available() > 0) {
		incomingByte = Serial1.read();
          
                for (int count =1;count<14;count++)
                {serialBuffer[count-1]=serialBuffer[count]; } //slide buffer right
                serialBuffer[13]=incomingByte;
                 
                if (serialBuffer[0]==126&serialBuffer[2]==10&serialBuffer[3]==131) //filter to find Xbee default message structure
                {intensity =  serialBuffer[11]*256+ serialBuffer[12];               //Next time I'll use an actual library!
                intensity = floor(intensity*3/4); //clamping from Jan15 hardware hitting current limits at high intensity
                 focus = 20;
                 flickerSpeed =  50;//floor((intensity)/2);
                  // cool = floor((1024-intensity)/2);
                  //defines how hot our fire is burning 0-1024 
                   // 0 =0 no fire
                   // 100 flickering
                   // 500 red flames
                   // 700 yellow flames
              
                   stageOneGain =intensity*2; 
                if (stageOneGain>1024) stageOneGain=1204; //so gain winds up twice as fast to midrange, and then caps
                stageOneGain = stageOneGain*32;//max intensity is 65536 or 256*256, or in this case 1024*64
                 if (intensity>512) {stageTwoGain = (512+intensity/2)*64;} else {stageTwoGain=0;} //gain winds up from zero below intensity 512 to 32767 (1024*32) at intensity 1024     
                stageTwoOffset =  0.4*(intensity-512)/512; //adjusts the -1 to +1 noise values with a positive offset, this is why stage one gain only applies up to 32767, rather than 65536
                for (int i=0;i<10;i++) {Serial1.write(intensity);}
                Serial.println(intensity);}
	}
  
 }

void loop() {
   checkSerial();
   if ((millis()/1000)%2>0) digitalWrite(13,HIGH);else digitalWrite(13,LOW);  //debug LED blink
  if (msec > 7) {// flame update rate
     msec=0;
    if (lowBatLockout==0)  animateFire(); else colorWipe(0,10); //low voltage condition, disable light show
    
    tick++;
    analogueAvBuffer = analogueAvBuffer + analogRead(batMonPin );
    battMonReadCounter++;  
    if (64<battMonReadCounter) {
        battMonReadCounter=0; 
        if (analogueAvBuffer>>6<550) lowBatLockout =1;// else lowBatLockout =0; //else removed because of cyclic battery behavior under load
        Serial.println(analogueAvBuffer>>6);                                     //now latches in state until power cycled.
        analogueAvBuffer=0;    
      } 
         if (heartBeatDirection==0){  //and the longest chunk of code in the main loop makes a light blink...
            heartBeat++;
            if (heartBeat==255) heartBeatDirection =1;            
        } else{
             heartBeat--;
            if (heartBeat==0) heartBeatDirection =0;            
        } 
        if (lowBatLockout==0) analogWrite(statusLED ,heartBeat); 
        else analogWrite(statusLED ,(heartBeat>>2)%2*200);
  
  } 
  
}


void animateFire() {
  int i, c, n, x, y;
     float TickNoiseTemp = float(tick/flickerSpeed)-cos(float(tick)/flickerSpeed);     // work out this value once, rather than each cycle through the for loop
     for (y=height-1; y >= 0; y--) {
       float YnoiseTemp = float(y/2)-sin(float(y/2));                                 //
       for ( x=width-1; x >= 0; x--) { 
            int cellheat = 0;
            if (x>0) {cellheat = canvas[(y) * width + x];} 
            else //we are on the bottom row and need to feed some noise into the system
             {
               cellheat = floor(float(pnoise( YnoiseTemp,TickNoiseTemp,5)*stageOneGain)); //pnoise is a perlin noise routine taking three axis and returning floats -1 to +1
               if (intensity>512) {                                                       //we then multiply those by the gain values to produce numbers 0-65536
                   int tempheat=floor((float(pnoise( YnoiseTemp,TickNoiseTemp,10)+stageTwoOffset)*stageTwoGain)); 
                   if (tempheat>cellheat) cellheat =tempheat; //adds second layer to the hot spots above half scale input
               }    
           }            
            float tempfloat = cellheat;
            tempfloat = 10-cellheat/8000; //coupling facter based on having a multiple between 10 and ~2 (cell heat will be between 0 and 65536 
            int HeatTransfer = ceil(float(cellheat)/tempfloat);//for high heat values we move them up fast taking half each clock cycle. Cooler temps move 1/10th and rounded up
            canvas[(y) * width + x]=cellheat-HeatTransfer;  
           if(x<width-1) canvas[(y) * width + x+1]=HeatTransfer+canvas[(y) * width + x+1]; //error trap to avoid writing off the top of the array
            
       }
     }

  // Step 3: interpolate
  //Averages with the four cells around it, error checking as it goes to avoid leaving array boundries.
  for (y=0; y < height; y++) {
    for (x=0; x < width; x++) {
      c = canvas[y * width + x] * focus;
      n = focus;
      if (x > 0) {
        c = c + canvas[y * width + (x - 1)];
        n = n + 1;
      }
      if (x < width-1) {
        c = c + canvas[y * width + (x + 1)];
        n = n + 1;
      }
      if (y > 0) {
        c = c + canvas[(y -1) * width + x];
        n = n + 1;
      }
      if (y < height-1) {
        c = c + canvas[(y + 1) * width + x];
        n = n + 1;
      }
      c = (c + (n / 2)) / n;
      i = (random(1000) * cool)*x / 10000;
      if (c > i) {
        c = c - i;
      } else {
        c = 0;
      }
      canvas[y * width + x] = c;
    }
  }
  
  // Step 4: render canvas to LEDs
  for (y=0; y < height; y++) {
    for (x=0; x < width; x++) {
      c = floor(canvas[((height - 1) - y) * width + x]>>8);
      if (c<0) c = 0;
      if (c>100) c =99;
      //c = 10;
     // c = ((x*6)*256*256)+y*0;
    // leds.setPixel(xy(x, y), 5);
       if (y%2==0) leds.setPixel(xy(x, y), fireColor[c]); else leds.setPixel(xy(11-x, y), fireColor[c]);//because I missed the way xy() handles even lines
    }
  }
  leds.show();
}
//debug routine now used to blank array in low battery condition
void colorWipe(int color, int wait)
{
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(wait);
  }
}
