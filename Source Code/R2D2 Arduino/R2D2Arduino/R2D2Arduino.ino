#include <SPI.h>
#include <Ethernet.h>
#include <MP3Trigger.h>
#include <SendOnlySoftwareSerial.h>
#include <Wire.h>

#define MAX_FRAME_LENGTH 64

#include <WebSocket.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xE1, 0xB5 };
byte ip[] = { 82, 50, 68, 51 };
//byte ip[] = { 192, 168, 1, 151 };

// WebSocket variables
WebSocket wsServer;

// MP3Trigger variables
SendOnlySoftwareSerial bodyMp3TriggerSerial(3);
SendOnlySoftwareSerial domeMp3TriggerSerial(5);
SendOnlySoftwareSerial feetMp3TriggerSerial(9);
MP3Trigger bodyMp3Trigger;
MP3Trigger domeMp3Trigger;
MP3Trigger feetMp3Trigger;

#define MAX_ARRAY_LENGTH 20
byte gamePadState[MAX_ARRAY_LENGTH];

void setup() {
  Wire.begin();
  
//  WebSocketSetup();
//  MP3TriggerSetup();
  
  // once all setup is done we are going to begin processing so play a sound
//  bodyMp3Trigger.play(52);  

  pinMode(13, OUTPUT);
}

void loop() {
  Wire.beginTransmission(20);
  Wire.write(1);
  Wire.endTransmission();
  digitalWrite(13, HIGH);
  delay(500);
  
  Wire.beginTransmission(20);
  Wire.write(2);
  Wire.endTransmission();
  digitalWrite(13, LOW);
  delay(500);
  
  
//  wsServer.listen();
  
//  processPS2();
/*
  Wire.beginTransmission(128);
  Wire.write(0);
  Wire.write(0);
  Wire.write(1);
  Wire.endTransmission();
  delay(5000);
  
  Wire.beginTransmission(128);
  Wire.write(1);
  Wire.write(0);
  Wire.write(2);
  Wire.endTransmission();
  delay(5000);

  Wire.beginTransmission(128);
  Wire.write(2);
  Wire.write(0);
  Wire.write(3);
  Wire.endTransmission();
  delay(5000);
  */
}

// ----------------------------------------------------------
// WebSocket
// ----------------------------------------------------------
void WebSocketSetup() {
  Ethernet.begin(mac, ip);
  
  wsServer.registerConnectCallback(&onConnect);
  wsServer.registerDataCallback(&onData);
  wsServer.registerDisconnectCallback(&onDisconnect);  
  wsServer.begin();
  
  delay(100);
}

void onConnect(WebSocket &socket) {
  //bodyMp3Trigger.play(13);
}

void onDisconnect(WebSocket &socket) {
  //bodyMp3Trigger.play(20);
}

void onData(WebSocket &socket, char* dataString, byte frameLength) {
  // split the data 
  byte i2cCommand[12];
  int i2cIndex = 0;
  char *savePointer;
  char *token;
            
  for (token = strtok_r(dataString, "/", &savePointer); token; token = strtok_r(NULL, "/", &savePointer))
  {
    i2cCommand[i2cIndex] = atoi(token);
    i2cIndex++;
  }
            
  // I2C device command            
  if (i2cCommand[0] < 127) {
    Wire.beginTransmission(i2cCommand[0]);
    for (int i = 1; i < i2cIndex; i++)
      Wire.write(i2cCommand[i]);
    Wire.endTransmission();
  } 
  
  // Body MP3 command
  else if (i2cCommand[0] == 128) {
    // 128 is body MP3
    bodyMp3Trigger.play(i2cCommand[1]);
  } else if (i2cCommand[0] == 129) {
    // 129 is body volume
    bodyMp3Trigger.setVolume(i2cCommand[1]);
  }  
}

// ----------------------------------------------------------
// MP3Trigger 
// ----------------------------------------------------------
void MP3TriggerSetup() {
  bodyMp3Trigger.setup(&bodyMp3TriggerSerial);
  bodyMp3TriggerSerial.begin(MP3Trigger::serialRate());
  bodyMp3Trigger.setVolume(0);
  domeMp3Trigger.setup(&domeMp3TriggerSerial);
  domeMp3TriggerSerial.begin(MP3Trigger::serialRate());
  domeMp3Trigger.setVolume(0);
  feetMp3Trigger.setup(&feetMp3TriggerSerial);
  feetMp3TriggerSerial.begin(MP3Trigger::serialRate());
  feetMp3Trigger.setVolume(0);
}

void processPS2() {
  // send the command
  Wire.requestFrom(110,20);
  
  int index = 0;
  while (Wire.available()) {
    gamePadState[index] = Wire.read();
    index++;
  }
  
  if (gamePadState[4] == 1)
    bodyMp3Trigger.play(52);  
  
}
