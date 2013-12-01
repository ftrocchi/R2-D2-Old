#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <Wire.h>
#include <LedControl.h>
#include "AdapterType.h"
#include "WireAddress.h"
#include "DeviceCount.h"
#include "DeviceBrightness.h"
#include "TextDisplay.h"

// SET THESE MANUALLY
int logicsI2CAdapter = RLD;
int deviceCount = RLDDeviceCount;
LedControl ledControl = LedControl(2, 4, 8, deviceCount);
unsigned char logic[RLDDeviceCount][6];
bool vLogic[RLDDeviceCount][5][9];

int delayTime = 10;

void setup() {
  setupLogics();
  
  Wire.onReceive(receiveEvent);
}

void loop() {
}

void setupLogics() {
  for (int device = 0; device < ledControl.getDeviceCount(); device++) {
    ledControl.shutdown(device, false);
    ledControl.clearDisplay(device);
  }

  switch (logicsI2CAdapter) {
    case RLD:
      Wire.begin(RLDAddress);
      ledControl.setIntensity(0, LDBrightness);
      ledControl.setIntensity(1, LDBrightness);
      ledControl.setIntensity(2, LDBrightness);
      ledControl.setIntensity(3, PSIBrightness);
      displayString("ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 - !.+?#~>_<@^£$& abcdefghijklmnopqrstuvwxyz");
      FeedGridLeft(B00000); break;
    case FLD:
      Wire.begin(FLDAddress);
      ledControl.setIntensity(0, LDBrightness);
      ledControl.setIntensity(1, LDBrightness);
      ledControl.setIntensity(2, PSIBrightness);
      FeedGridLeft(B00000); break;
    case CBI:
      Wire.begin(CBIAddress);
      ledControl.setIntensity(0, CBIBrightness);
      ledControl.setIntensity(1, DPBrightness);
      FeedGridLeft(B00000); break;
  }
}

void receiveEvent(int eventCode) {
}

void displayString(char text[]) {
  for (unsigned char i = 0; i < strlen(text); i++)
    drawLetter(text[i]);
}

void drawLetter(char letter) {
  if (letter >= 'a' && letter  <= 'z')
    letter = toupper(letter);
  
  switch (letter) {
    case 'A' : FeedGridLeft(B11110); FeedGridLeft(B00101); FeedGridLeft(B00101); FeedGridLeft(B11110); FeedGridLeft(B00000); break;
    case 'B' : FeedGridLeft(B11111); FeedGridLeft(B10101); FeedGridLeft(B10101); FeedGridLeft(B01010); FeedGridLeft(B00000); break;
    case 'C' : FeedGridLeft(B01110); FeedGridLeft(B10001); FeedGridLeft(B10001); FeedGridLeft(B10001); FeedGridLeft(B00000); break;
    case 'D' : FeedGridLeft(B11111); FeedGridLeft(B10001); FeedGridLeft(B10001); FeedGridLeft(B01110); FeedGridLeft(B00000); break;
    case 'E' : FeedGridLeft(B11111);FeedGridLeft(B10101); FeedGridLeft(B10001); FeedGridLeft(B00000); break;
    case 'F' : FeedGridLeft(B11111);FeedGridLeft(B00101); FeedGridLeft(B00001); FeedGridLeft(B00000); break;
    case 'G' : FeedGridLeft(B01110); FeedGridLeft(B10001); FeedGridLeft(B11001); FeedGridLeft(B01010); FeedGridLeft(B00000); break;
    case 'H' : FeedGridLeft(B11111); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B11111); FeedGridLeft(B00000); break;
    case 'I' : FeedGridLeft(B10001);FeedGridLeft(B11111); FeedGridLeft(B10001); FeedGridLeft(B00000); break;
    case 'J' : FeedGridLeft(B01000); FeedGridLeft(B10001); FeedGridLeft(B01111); FeedGridLeft(B00001); FeedGridLeft(B00000); break;
    case 'K' : FeedGridLeft(B11111);FeedGridLeft(B00100); FeedGridLeft(B11011); FeedGridLeft(B00000); break;
    case 'L' : FeedGridLeft(B11111);FeedGridLeft(B10000); FeedGridLeft(B10000); FeedGridLeft(B00000); break;
    case 'M' : FeedGridLeft(B11111); FeedGridLeft(B00010); FeedGridLeft(B00100); FeedGridLeft(B00010); FeedGridLeft(B11111); FeedGridLeft(B00000); break;
    case 'N' : FeedGridLeft(B11111); FeedGridLeft(B00110); FeedGridLeft(B01100); FeedGridLeft(B11111); FeedGridLeft(B00000); break;
    case 'O' : FeedGridLeft(B01110); FeedGridLeft(B10001); FeedGridLeft(B10001); FeedGridLeft(B01110); FeedGridLeft(B00000); break;
    case 'P' : FeedGridLeft(B11111); FeedGridLeft(B00101); FeedGridLeft(B00101); FeedGridLeft(B00010); FeedGridLeft(B00000); break;
    case 'Q' : FeedGridLeft(B01110); FeedGridLeft(B10001); FeedGridLeft(B11001); FeedGridLeft(B11110); FeedGridLeft(B00000); break;
    case 'R' : FeedGridLeft(B11111); FeedGridLeft(B00101); FeedGridLeft(B00101); FeedGridLeft(B11010); FeedGridLeft(B00000); break;
    case 'S' : FeedGridLeft(B10110);FeedGridLeft(B10101); FeedGridLeft(B01101); FeedGridLeft(B00000); break;
    case 'T' : FeedGridLeft(B00001);FeedGridLeft(B11111); FeedGridLeft(B00001); FeedGridLeft(B00000); break;
    case 'U' : FeedGridLeft(B01111); FeedGridLeft(B10000); FeedGridLeft(B10000); FeedGridLeft(B01111); FeedGridLeft(B00000); break;
    case 'V' : FeedGridLeft(B01111);FeedGridLeft(B11000); FeedGridLeft(B01111); FeedGridLeft(B00000); break;
    case 'W' : FeedGridLeft(B11111); FeedGridLeft(B01000); FeedGridLeft(B00110); FeedGridLeft(B01000); FeedGridLeft(B11111); FeedGridLeft(B00000); break;
    case 'X' : FeedGridLeft(B10001); FeedGridLeft(B11011); FeedGridLeft(B00100); FeedGridLeft(B11011); FeedGridLeft(B10001); FeedGridLeft(B00000); break;
    case 'Y' : FeedGridLeft(B00011);FeedGridLeft(B11110); FeedGridLeft(B00011); FeedGridLeft(B00000); break;
    case 'Z' : FeedGridLeft(B10001); FeedGridLeft(B11101); FeedGridLeft(B10111); FeedGridLeft(B10001); FeedGridLeft(B00000); break;

    case '0' : FeedGridLeft(B01110); FeedGridLeft(B10001); FeedGridLeft(B01110); FeedGridLeft(B00000); break;
    case '1' : FeedGridLeft(B10010); FeedGridLeft(B11111); FeedGridLeft(B10000); FeedGridLeft(B00000); break;
    case '2' : FeedGridLeft(B10010); FeedGridLeft(B11001); FeedGridLeft(B10110); FeedGridLeft(B00000); break;
    case '3' : FeedGridLeft(B10001); FeedGridLeft(B10101); FeedGridLeft(B01010); FeedGridLeft(B00000); break;
    case '4' : FeedGridLeft(B01100); FeedGridLeft(B01010); FeedGridLeft(B11111); FeedGridLeft(B01000); FeedGridLeft(B00000); break;
    case '5' : FeedGridLeft(B10111); FeedGridLeft(B10101); FeedGridLeft(B01001); FeedGridLeft(B00000); break;
    case '6' : FeedGridLeft(B01110); FeedGridLeft(B10101); FeedGridLeft(B01001); FeedGridLeft(B00000); break;
    case '7' : FeedGridLeft(B10001); FeedGridLeft(B01101); FeedGridLeft(B00011); FeedGridLeft(B00000); break;
    case '8' : FeedGridLeft(B01010); FeedGridLeft(B10101); FeedGridLeft(B10101); FeedGridLeft(B01010); FeedGridLeft(B00000); break;
    case '9' : FeedGridLeft(B10010); FeedGridLeft(B10101); FeedGridLeft(B01110); FeedGridLeft(B00000); break;
    
    case '-' : FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00000); break;
    case ' ' : FeedGridLeft(B00000); FeedGridLeft(B00000); FeedGridLeft(B00000); break;
    case '!' : FeedGridLeft(B10111); FeedGridLeft(B00000); break;
    case '.' : FeedGridLeft(B10000); FeedGridLeft(B00000); break;
    case '+' : FeedGridLeft(B00011); FeedGridLeft(B00000); break;
    case '?' : FeedGridLeft(B00001); FeedGridLeft(B10101); FeedGridLeft(B00010); FeedGridLeft(B00000); break;
    case '#' : FeedGridLeft(B00000); FeedGridLeft(B10000); FeedGridLeft(B11100); FeedGridLeft(B01110); FeedGridLeft(B11111); FeedGridLeft(B11101); FeedGridLeft(B10011); FeedGridLeft(B00000); FeedGridLeft(B00000); break;
    case '~' : FeedGridLeft(B10100); FeedGridLeft(B01110); FeedGridLeft(B00101); FeedGridLeft(B00101); FeedGridLeft(B01110); FeedGridLeft(B10100); break;
    case '>' : FeedGridLeft(B00100); FeedGridLeft(B01000); FeedGridLeft(B11111); FeedGridLeft(B00010); FeedGridLeft(B00010); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00100); break;
    case '_' : FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00100); break;
    case '<' : FeedGridLeft(B11100); FeedGridLeft(B11111); FeedGridLeft(B11000); FeedGridLeft(B11100); FeedGridLeft(B10000); FeedGridLeft(B11110); FeedGridLeft(B11000); FeedGridLeft(B11111); FeedGridLeft(B11110); FeedGridLeft(B11111); 
               FeedGridLeft(B11000); FeedGridLeft(B11100); FeedGridLeft(B11000); FeedGridLeft(B10000); FeedGridLeft(B11100); FeedGridLeft(B11111); FeedGridLeft(B11100); FeedGridLeft(B11111); FeedGridLeft(B11100); FeedGridLeft(B10000); 
               FeedGridLeft(B11111); FeedGridLeft(B11000); FeedGridLeft(B10000); FeedGridLeft(B10000); FeedGridLeft(B11110); break;
    case '@' : FeedGridLeft(B00100); FeedGridLeft(B00010); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B01000); FeedGridLeft(B10000); FeedGridLeft(B01000); FeedGridLeft(B00100); FeedGridLeft(B00010); FeedGridLeft(B00001); 
               FeedGridLeft(B00010); FeedGridLeft(B00100); FeedGridLeft(B00100); FeedGridLeft(B00010); FeedGridLeft(B00100); FeedGridLeft(B01000); FeedGridLeft(B10000); FeedGridLeft(B01000); FeedGridLeft(B01000); FeedGridLeft(B10000); 
               FeedGridLeft(B01000); FeedGridLeft(B00110); FeedGridLeft(B00001); FeedGridLeft(B00001); FeedGridLeft(B00010); FeedGridLeft(B00010); FeedGridLeft(B00100); FeedGridLeft(B01000); FeedGridLeft(B10000); FeedGridLeft(B01000); 
               FeedGridLeft(B01000); FeedGridLeft(B10000); FeedGridLeft(B01000); FeedGridLeft(B00100); FeedGridLeft(B00010); FeedGridLeft(B00010); break;
    case '^' : FeedGridLeft(B11111); FeedGridLeft(B10011); FeedGridLeft(B10101); FeedGridLeft(B11001); FeedGridLeft(B10101); FeedGridLeft(B10011); FeedGridLeft(B11111); FeedGridLeft(B00000); break;
    case '£' : FeedGridLeft(B10001); FeedGridLeft(B11011); FeedGridLeft(B11101); FeedGridLeft(B01110); FeedGridLeft(B00000); break;
    case '$' : FeedGridLeft(B11110); FeedGridLeft(B01101); FeedGridLeft(B11111); FeedGridLeft(B01101); FeedGridLeft(B11110); FeedGridLeft(B00000); break;
    case '&' : FeedGridLeft(B00100); FeedGridLeft(B00000); FeedGridLeft(B00100); FeedGridLeft(B00000); break;    
  }
}

void FeedGridLeft(unsigned char newColumn) {
  bool temp8[5], temp17[5];
  
  for (int row = 0; row < 5; row++) {
    temp8[row] = vLogic[2][row][8];
    temp17[row] = vLogic[1][row][8];
    
    for (int led = 0; led < 8; led++) {
      for (int device = 0; device < deviceCount -1; device++) {
        vLogic[device][row][8-led] = vLogic[device][row][7-led];
      }
    }    
    
    for (int device = 0; device < deviceCount -1; device++) {
      vLogic[device][row][0] = temp8[row];
    }
    
    if (logicsI2CAdapter == RLD) {
      vLogic[0][row][0] = temp17[row];
      vLogic[2][row][0] = (( newColumn >> row) & 1);
    }
  }
  
  mapBoolGrid();
  displayLogic();
}

void mapBoolGrid() {
  for (int printRow = 0; printRow < 5; printRow++) {
    for (int led = 0; led < 8; led++) {
      if (led < 5) {
        for (int device = 0; device < deviceCount -1; device++) {
          logic[device][5] = 2 * logic[device][5] + vLogic[device][led][0];
        }
      }
      
      for (int device = 0; device < deviceCount -1; device++) {
        logic[device][printRow] = 2 * logic[device][printRow] + vLogic[device][printRow][8-led];
      }
    }
  }
  
  for (int device = 0; device < deviceCount -1; device++) {
    logic[device][5] = 8 * logic[device][5];
  }
}

void displayLogic() {
  for (int i=0; i<6; i++) {
    for (int device = 0; device < deviceCount -1; device++)
      ledControl.setRow(device, i, logic[device][i]);
    
    delay(delayTime);
  }
}

