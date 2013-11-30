#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <Wire.h> //used to support I2C
#include <Servo.h>// Supports 2 Servos 1A (hp1) & 1B (hp2)
#include "LEDState.h"
#include "LEDMode.h"

#define VERSION 2
#define HP 3    // 1=Front FHP (BLUE), 2=Top THP (RED), 3=Rear RHP (GREEN)
#define redPin    10 // blue
#define greenPin  11 // red
#define bluePin   9  // green
#define systemLED 13

unsigned long t=0;
int randomLEDFX;

long loopCount = 0;
long twitchLEDTime = 4525; // Long??
long twitchHPTime = 4525; // Long??

Servo hp1servo;   // HP 1A default
Servo hp2servo;   // HP 1B default

LEDState currentState = OFF;
LEDMode currentMode = COLOR;
bool twitchOn = false;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  if (HP==1) {
    Wire.begin(25);
    twitchLEDTime = 1000;
    twitchHPTime=2500;
    blueON(5000);
  } // Start I2C Bus as Master I2C Address 25 (Assigned FHP)
  if (HP==2) {
    Wire.begin(26);
    twitchLEDTime = 2300;
    twitchHPTime=1750;
    redON(5000);
  }  // Start I2C Bus as Master I2C Address 26 (Assigned THP)
  if (HP==3) {
    Wire.begin(27);
    twitchLEDTime = 1900;
    twitchHPTime=4500;
    greenON(5000);
  }// Start I2C Bus as Master I2C Address 27 (Assigned RHP)

  Wire.onReceive(receiveEvent);            // register event so when we receive something we jump to receiveEvent();
  Wire.onRequest(requestEvent);

  hp1servo.attach(3);  // attaches the servo on Digital Out pin 3 to the servo object 
  hp2servo.attach(5);  // attaches the servo on Digital Out pin 3 to the servo object 
  delay(1000);         // allow time for the servos to attach
  centerHP();
  testLED();
  //  testSERVOS();

}

void loop(){
  if (twitchOn) {
    loopCount++;
    t=millis();
    if (t > twitchLEDTime)
    {
      twitchLED();     // call playRandom routine
      loopCount = 0;                       // reset loopCount
      twitchLEDTime = (random(1,10)*500)+millis();   // set the next twitchTime   
    }
    if (t > twitchHPTime)
    {
      twitchHP();     // call playRandom routine
      loopCount = 0;                       // reset loopCount
      twitchHPTime = (random(1,10)*750)+millis();   // set the next twitchTime   
    }
  }
}

void testLED(){
  redON(1000);
  greenON(1000);
  blueON(1000);
  magentaON(1000);
  yellowON(1000);
  cyanON(1000);
  whiteON(1000);
  ledOFF(1000);
}

void alarm(int timed){
  int HPalarmdelay=1000;
  unsigned long timeNew=millis();
  unsigned long flip = timeNew + (timed*1000);  
  while (timeNew < flip && currentMode == ALARM) {
    redON(HPalarmdelay);
    ledOFF(HPalarmdelay/2);
    timeNew=millis();
  }
  
  currentMode = COLOR;
}

void HoloMessage(int MessageLength){
  if (HP==1){ 
    lookdownHP();
  }
  t=millis();
  unsigned long lmt=t + (MessageLength*1000);
  while (t < lmt && currentMode == LEIA){
    cyanON(random(1,10)*10);
    whiteON(random(1,5)*10);
    blueON(random(4,9)*10);
    t=millis();
  }
  ledOFF();
  
  currentMode = COLOR;
}

void Disco(int dancetime, int bpm){
  unsigned long DanceBeatDelay = 60000/bpm;
  unsigned long timeNew= millis();
  unsigned long gottadance = (dancetime*1000) + timeNew;

  while (timeNew < gottadance && currentMode == DISCO){
    whiteON(DanceBeatDelay);
    ledOFF(DanceBeatDelay);
    timeNew= millis();
  }
  
  currentMode = COLOR;
}

void SystemFailure(){
  int failureDelay = 200;
  
  redON(failureDelay);
  blueON(failureDelay);
  magentaON(failureDelay);
  yellowON(failureDelay);
  cyanON(failureDelay);
  whiteON(failureDelay);
  
  currentMode = COLOR;
}

void setLED(int redValue, int greenValue, int blueValue, int timed, LEDState state) {
  currentState = state;
  digitalWrite(redPin, redValue);
  digitalWrite(greenPin, greenValue);
  digitalWrite(bluePin, blueValue);
  if (timed > 0) {
    delay(timed);
    ledOFF();
  }
}

void ledOFF() {
  ledOFF(0);
}

void ledOFF(int timed) { 
  setLED(LOW, LOW, LOW, timed, OFF); 
}
void whiteON(int timed) { 
  setLED(HIGH, HIGH, HIGH, timed, WHITE); 
}
void redON(int timed) { 
  setLED(HIGH, LOW, LOW, timed, RED); 
}
void greenON(int timed){ 
  setLED(LOW, HIGH, LOW, timed, GREEN); 
}
void blueON(int timed){ 
  setLED(LOW, LOW, HIGH, timed, BLUE); 
}
void cyanON(int timed){ 
  setLED(LOW, HIGH, HIGH, timed, CYAN); 
}
void yellowON(int timed){ 
  setLED(HIGH, HIGH, LOW, timed, YELLOW); 
}
void magentaON(int timed){ 
  setLED(HIGH, LOW, HIGH, timed, MAGENTA); 
}

void twitchLED() {
  randomLEDFX = random (1,13);    // Pick a number 1 - 7
  switch (randomLEDFX) {
  case 1:
    whiteON(0);
    break;
  case 2:
    ledOFF();
    break;
  case 3:
    redON(0);
    break;
  case 4:
    magentaON(0);
    break;
  case 5:
    greenON(0);
    break;
  case 6:
    blueON(0);
    break;
  case 7:
    cyanON(0);
    break;
  case 8:
    yellowON(0);
    break;
  case 9:
    ledOFF();
    break;
  case 10:
    ledOFF();
    break;
  default:
    break;
  }
}

// --------------------------------------------------------------------------------------
// Receive / Request
// --------------------------------------------------------------------------------------
void receiveEvent(int eventCode) {
  int i2cEvent=Wire.read();
  sei();
  
  // prep
  if ((i2cEvent < 9 && i2cEvent != 3) || i2cEvent == 13)
    currentMode = COLOR;
   
  switch (i2cEvent) {
  case 1:
    whiteON(0);
    break;
  case 2:
    ledOFF();
    break;
  case 3:
    currentMode = ALARM;
    alarm(6); 
    break;
  case 4:
    redON(0);
    break;
  case 5:
    greenON(0);
    break;
  case 6:
    blueON(0);
    break;
  case 7:
    cyanON(0);
    break;
  case 8:
    yellowON(0);
    break;
  case 9:
    currentMode = LEIA;
    HoloMessage(31);
    break;
  case 10:
    currentMode = DISCO;
    Disco(10,120);
    break;
  case 11:
    currentMode = SYSTEMFAILURE;
    SystemFailure();
    break;
  case 12:
    twitchOn = !twitchOn;
    break;
  case 13:
    magentaON(0);
    break;
  case 20:
    { 
    byte xValue = Wire.read();
    byte yValue = Wire.read();
    setHPLocation(xValue, yValue);
    }
    break;

  default: 
    // if nothing else matches, do the default
    // so we are going to do nothing... for that matter not even waste time
    break;
  }
}

void requestEvent() {
  byte returnValue[2];
  
  returnValue[0] = VERSION; 
  returnValue[1] = currentState;
  
  Wire.write(returnValue, 2);
}

// --------------------------------------------------------------------------------------
// SERVOS
// --------------------------------------------------------------------------------------
void testSERVOS(){
  hp1servo.write(90);
  hp2servo.write(90);
  delay(500);          // wait for a .5 secs for the servos to move
}

void twitchHP(){
  hp1servo.write(random(20,150));
  hp2servo.write(random(20,150));
  delay(500);
}

void centerHP(){
  hp1servo.write(90);
  hp2servo.write(90);
  delay(500);          // wait for a .5 secs for the servos to move
}

void lookdownHP(){
  hp1servo.write(20);
  hp2servo.write(90);
  delay(500);          // wait for a .5 secs for the servos to move
}

void setHPLocation(byte x, byte y) {
  hp1servo.write(y);
  hp2servo.write(x);
}
