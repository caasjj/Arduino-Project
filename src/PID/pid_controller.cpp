/**********************************************************************************************
 * Arduino PID Controller Example - Version 0.0.1
 * by Walid Hosseini <caasjj@gmail.com> walidhosseini.com
 *
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/
#include "Arduino.h"
#include "pid_controller.h"

PID 		*_pid;
Messager 	*_messager;
Command 	*_command;

void onNewSample(PIDState *pidState) {

 _messager->sendMessage(STATUS_MSG, (byte *) &pidState->adcInput, (unsigned int) sizeof(*pidState) );

}

void onReceivedCommand(Command *command) {

	TestMsg testMsg;

	testMsg.code = (uint16_t) command->cmdType;
	testMsg.value = (uint16_t) command->body[0];

	_messager->sendMessage(TEST_MSG, (byte *) &testMsg, (unsigned int) sizeof(testMsg) );

}

void setup() {

  // Configure the PID to callback 'onNewSample' after each sample, which may or may
  // not be a time of PID update depending on pidUpdateRate
  PIDConfig pidConfig;
  pidConfig.adcChannel       = ADC_CHANNEL;
  pidConfig.adcSampleRateHz  = ADC_SAMPLE_RATE_HZ;
  pidConfig.loopUpdateRatio  = PID_LOOP_UPDATE_RATIO;
  pidConfig.diagLedPin       = PID_DIAG_LED_PIN;
  pidConfig.callback         = &onNewSample;
  _pid 						 = new PID( pidConfig );

  // Configure the Serial port to receive commands and send messages
  MSGConfig msgConfig;
  msgConfig.baudRate 		 = SERIAL_BAUD_RATE;
  msgConfig.callback 		 = &onReceivedCommand;
  _messager 				 = new Messager( msgConfig );

}

void loop() {

}
