#include "PSI.h"

const int PSI::patternAtStage[9] = { B01010101, B11010101, B10010101, B10110101, B10101010, B00101010, B01101010, B01010101, B01010101 };

PSI::PSI(int address, LedControl *led, int deviceIndex, int first, int second, int transition) {
  i2cAddress = address;
  ledControl = led;
  device = deviceIndex;
  
  ResetDelays(first, second, transition);
}

PSI::~PSI() {
}

void PSI::ProcessCommand() {
  I2C_PSI_Commands::Value command = (I2C_PSI_Commands::Value)Wire.read();
  
  switch (command) {
    case I2C_PSI_Commands::On:
      On();
      break;
      
    case I2C_PSI_Commands::Off:
      Off();
      break;
      
    case I2C_PSI_Commands::Brightness:
      int brightness = Wire.read();
      SetBrightness(brightness);
      break;
  }
}

void PSI::On() {
  for (int row = 0; row < 6; row ++) {
    ledControl->setRow(device, row, 255);
  }
}

void PSI::Off() {
  ledControl->clearDisplay(device);
}

void PSI::SetBrightness(int brightness) {
  ledControl->setIntensity(device, brightness);  
}

void PSI::ResetDelays(int first, int second, int transition) {
  delayAtStage[0] = first;
  delayAtStage[1] = transition / 3; 
  delayAtStage[2] = delayAtStage[1];
  delayAtStage[3] = delayAtStage[1]; 
  delayAtStage[4] = second; 
  delayAtStage[5] = delayAtStage[1]; 
  delayAtStage[6] = delayAtStage[1]; 
  delayAtStage[7] = delayAtStage[1]; 
  delayAtStage[8] = first; 
  
  stage = 0;
  slideDirection = 1;
  maxStage = 8;
  lastTimeCheck = 0;
}

void PSI::Animate() {
  unsigned long timeNow = millis();
  
  // early exit if we don't need to do anything
  if (timeNow - lastTimeCheck < delayAtStage[stage])
    return;
  
  // set the time  
  lastTimeCheck = timeNow;
  
  // move to next stage
  stage += slideDirection;
  
  // bounds check for stage
  if (stage >= maxStage) {
    stage = maxStage;
    slideDirection = -1;
  } else if (stage <= 0) {
    stage = 0;
    slideDirection = 1;
  }
  
  // set the stage pattern
  ledControl->setRow(device, 0, patternAtStage[stage]);
  ledControl->setRow(device, 1, ~patternAtStage[stage]);
  ledControl->setRow(device, 2, patternAtStage[stage]);
  ledControl->setRow(device, 3, ~patternAtStage[stage]);
  ledControl->setRow(device, 4, patternAtStage[stage]);
}
