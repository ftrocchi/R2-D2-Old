#include <SendOnlySoftwareSerial.h>
#include <MP3Trigger.h>
#include <Wire.h>

enum MP3Board {
  Body,
  Dome,
  Feet,
  None
};

enum MP3Command {
  Play,
  Volume
};

SendOnlySoftwareSerial bodyMp3TriggerSerial(15);
MP3Trigger bodyMp3Trigger;
SendOnlySoftwareSerial domeMp3TriggerSerial(14);
MP3Trigger domeMp3Trigger;
SendOnlySoftwareSerial feetMp3TriggerSerial(16);
MP3Trigger feetMp3Trigger;

int RXLED = 17;

void setup() {
  pinMode(RXLED, OUTPUT); 

  bodyMp3Trigger.setup(&bodyMp3TriggerSerial);
  bodyMp3TriggerSerial.begin(MP3Trigger::serialRate());
  bodyMp3Trigger.setVolume(0);
  domeMp3Trigger.setup(&domeMp3TriggerSerial);
  domeMp3TriggerSerial.begin(MP3Trigger::serialRate());
  domeMp3Trigger.setVolume(0);
  feetMp3Trigger.setup(&feetMp3TriggerSerial);
  feetMp3TriggerSerial.begin(MP3Trigger::serialRate());
  feetMp3Trigger.setVolume(0);
  
  Wire.begin(128);
  Wire.onReceive(receiveEvent);
  
  bodyMp3Trigger.play(52);  
}

void loop() {
  delay(2000);
  SetLEDs(None);
}

void receiveEvent(int eventCode) {
  int mp3Board = Wire.read();
  int mp3Command = Wire.read();
  int mp3Value = Wire.read();
  
  SetLEDs(mp3Board);
  
  switch (mp3Command) {
    case Play:
      PlaySound(mp3Board, mp3Value);    
      break;
    case Volume:
      SetVolume(mp3Board, mp3Value);
      break;
  }
}

void PlaySound(int board, int value) {
  switch (board) {
    case Body:
      bodyMp3Trigger.play(value);
      break;
    case Dome:
      domeMp3Trigger.play(value);
      break;
    case Feet:
      feetMp3Trigger.play(value);
      break;
  }
}

void SetVolume(int board, int value) {
  switch (board) {
    case Body:
      bodyMp3Trigger.setVolume(value);
      break;
    case Dome:
      domeMp3Trigger.setVolume(value);
      break;
    case Feet:
      feetMp3Trigger.setVolume(value);
      break;
  }  
}

void SetLEDs(int board) {
  digitalWrite(RXLED, board == Body || board == Feet ? LOW : HIGH);
  board == Dome || board == Feet ?  TXLED1 : TXLED0;
}

