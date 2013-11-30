#include <PS2X_lib.h>
#include <Wire.h>

PS2X ps2x;
#define MAX_ARRAY_LENGTH 20
byte gamePadState[MAX_ARRAY_LENGTH];

// for LED 
unsigned long activeTime;
bool isActive = false;
int ledDelay = 10;

void setup() {
  Serial.begin(57600);
  Serial.println("Begin Setup");
  
  if (setupGamePad() != 0) {
    Serial.println("Setup Game Pad Failed!");
    while(true) {
      TXLED1;
      delay(100);
      TXLED0;
      delay(100);
   }
  }
  
  Wire.begin(110);
  Wire.onRequest(requestEvent);
  TXLED0;
}

void loop() {
  updateLED();
  ps2x.read_gamepad();
  
  // sticks
  gamePadState[0] = ps2x.Analog(PSS_LX);
  gamePadState[1] = ps2x.Analog(PSS_LY);
  gamePadState[2] = ps2x.Analog(PSS_RX);
  gamePadState[3] = ps2x.Analog(PSS_RY);
  
  // buttons
  gamePadState[4] = ps2x.Button(PSB_BLUE);
  gamePadState[5] = ps2x.Button(PSB_RED);
  gamePadState[6] = ps2x.Button(PSB_GREEN);
  gamePadState[7] = ps2x.Button(PSB_PINK);
  
  // dpad
  gamePadState[8] = ps2x.Button(PSB_PAD_DOWN);
  gamePadState[9] = ps2x.Button(PSB_PAD_RIGHT);
  gamePadState[10] = ps2x.Button(PSB_PAD_UP);
  gamePadState[11] = ps2x.Button(PSB_PAD_LEFT);
  
  // other
  gamePadState[12] = ps2x.Button(PSB_SELECT);
  gamePadState[13] = ps2x.Button(PSB_START);
  gamePadState[14] = ps2x.Button(PSB_L1);
  gamePadState[15] = ps2x.Button(PSB_R1);
  gamePadState[16] = ps2x.Button(PSB_L2);
  gamePadState[17] = ps2x.Button(PSB_R2);
  gamePadState[18] = ps2x.Button(PSB_L3);
  gamePadState[19] = ps2x.Button(PSB_R3);
  
  delay(10);
}

int setupGamePad() {
  int error = ps2x.config_gamepad(15, 16, 14, 10, false, false); // clock, command, attention, data
  return error;
}

void requestEvent() {
  activateLED();
  Wire.write(gamePadState, MAX_ARRAY_LENGTH);
}

void activateLED() {
  TXLED1;
  isActive = true;
  activeTime = millis();
}

void updateLED() {
  if (isActive && millis() > activeTime + ledDelay) {
    TXLED0;
    isActive = false;
  }
}
