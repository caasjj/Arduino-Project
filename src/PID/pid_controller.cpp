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

 _messager->sendMessage(STATUS_MSG, (uint8_t *) &pidState->adcInput, (uint16_t) sizeof(*pidState) );

}

int8_t onReceivedCommand(Command *command) {

	int8_t status = 0;

	switch(command->cmdType) {

	case ENABLE_SAMPLER_CMD:
		status = _pid->EnableSampler();
		break;

	case DISABLE_SAMPLER_CMD:
		status = _pid->EnableSampler();
		break;

 	case CONFIGURE_PWM_CMD:
 		status = _pid->ConfigurePwm(command->body);
 		break;

 	case SET_LOOP_K_CMD:
 		status = _pid->SetLoopConstants(command->body);
 		break;

 	case GET_LOOP_K_CMD:
 		status = _pid->GetLoopConstants();
 		break;

 	case SET_OUTPUT_LIMITS_CMD:
 		status = _pid->SetOutputLimits(command->body);
 		break;

 	case SET_PWM_CMD:
 		status = _pid->SetPwm(command->body);
 		break;

 	case SET_SETPOINT_CMD:
 		status = _pid->SetSetpoint(command->body);
 		break;

 	case ENABLE_LOOP_CMD:
 		status = _pid->EnableLoop();
 		break;

 	case DISABLE_LOOP_CMD:
 		status = _pid->DisableLoop();
 		break;

 	case ENABLE_STATUS_MESSAGES_CMD:
 		status = _pid->EnableStatusMessages();
 		break;

 	case DISABLE_STATUS_MESSAGES_CMD:
 		status = _pid->DisableStatusMessages();
 		break;

	default: break;
	}

   return status;
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
