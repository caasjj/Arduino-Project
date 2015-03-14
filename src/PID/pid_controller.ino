/**********************************************************************************************
 * Arduino PID Controller Example - Version 0.0.1
 * by Walid Hosseini <caasjj@gmail.com> walidhosseini.com
 *
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/
#include "Arduino.h"
#include "pid_controller.h"

int count = 0;
byte enable = false;


typedef struct testMsg {
  ulong timeStamp;
  float data1;
  float data2;
  unsigned int count;
  bool enable;
};

testMsg    _testMsg;
  
void sendMessage(byte msgType, void *buffer, unsigned int length) {

  unsigned long delim = STREAM_DELIMITER;

  // Write the header
  byte *ptr = (byte *) &delim;
  for(int i=0; i<2; i++) {
    Serial.write(*ptr++);
  }

  // length takes 2 bytes, and message type 1 byte
  // so we have to write length+3 to the length field
  Serial.write( (length+3) >> 8);
  Serial.write( (length+3) & 0xFF);
  Serial.write( msgType );
  
  // write the body of the message
  ptr = (byte *) buffer;
  for(int i=0; i<length; i++) {
    Serial.write(*ptr++);
  }
  
  // write the trailer
  ptr = (byte *) &delim;
  for(int i=0; i<2; i++) {
    Serial.write(~(*ptr++));
  }
  
  Serial.flush();
}


void onUpdate(PIDState *pidState) {
  
  sendMessage(1, &pidState->timeStamp, 62u);

}

void setup() {
  
  PIDConfig config;
 
  config.diagLedPin       = DIAG_LED_PIN;
  config.adcChannel       = ADC_CHANNEL;
  config.adcSampleRateHz  = ADC_SAMPLE_RATE_HZ; 
  config.callback         = &onUpdate;
  
  PID* pid = new PID( config );

  _testMsg.timeStamp= millis();
  _testMsg.data1 = 1.0;
  _testMsg.data2 = 2.0;
  _testMsg.count = 5;
  _testMsg.enable = false;


  Serial.begin( SERIAL_BAUD_RATE );
}

byte messageBuffer[100];
unsigned int numBytes;
unsigned int messageLength;
byte messageType;
byte value = 0;
byte error = 13;
unsigned long lastTime = 0;

void loop() {
 
  if (millis() - _testMsg.timeStamp > 2000) {
    _testMsg.timeStamp = millis();
    sendMessage(3, &_testMsg, 15u);
    _testMsg.count += 1;
  } 
  
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
//void serialEvent() {
//  int i = 0;
//  while (Serial.available()) {
//    messageBuffer[i++] = Serial.read();
//  }
//  sendMessage(99, (void *) &i, 2);
//}
