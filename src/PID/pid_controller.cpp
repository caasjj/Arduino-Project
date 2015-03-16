/**********************************************************************************************
 * Arduino PID Controller Example - Version 0.0.1
 * by Walid Hosseini <caasjj@gmail.com> walidhosseini.com
 *
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/
#include "Arduino.h"
#include "pid_controller.h"

PID *_pid;
Messager *_messager;

void onUpdate(PIDState *pidState) {

 _messager->sendMessage(STATUS_MSG, (byte *) &pidState->adcInput, (unsigned int) sizeof(*pidState) );
 // _messager->sendMessage(0, (byte *) &pidState->adcInput, 58u );

}

void setup() {

  PIDConfig config;

  config.diagLedPin       = DIAG_LED_PIN;
  config.adcChannel       = ADC_CHANNEL;
  config.adcSampleRateHz  = ADC_SAMPLE_RATE_HZ;
  config.callback         = &onUpdate;

  _pid 			= new PID( config );
  _messager 	= new Messager( SERIAL_BAUD_RATE );

}

void loop() {

}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */

int _count = 0;
void serialEvent() {

  TestMsg testMsg;
  testMsg.size = Serial.available();

  for(int i=0; i<testMsg.size; i++) {
	Serial.read();
	_count++;
  }

  if (_count == 12) {
  	testMsg.count = _count;
  	_count = 0;
  	_messager->sendMessage(TEST_MSG, (byte *) &testMsg, (unsigned int) sizeof(testMsg));
  }

}
