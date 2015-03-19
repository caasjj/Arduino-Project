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

	int8_t 		status = 0;
	PIDLoopK	*pidLoopK_ptr;
//	TestMsg		testMsg;

//	uint8_t * ptr1;
//	uint8_t * ptr2;

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
 		_pid->SetLoopConstants(command->body);
 		pidLoopK_ptr = _pid->GetLoopConstants();
		_messager->sendMessage(LOOP_K_MSG, (uint8_t *)pidLoopK_ptr, sizeof(PIDLoopK));
		status = 0;
 		break;

 	case GET_LOOP_K_CMD:
 		pidLoopK_ptr = _pid->GetLoopConstants();
		_messager->sendMessage(LOOP_K_MSG, (uint8_t *)pidLoopK_ptr, sizeof(PIDLoopK));
		status = 0;
 		break;

 	case SET_OUTPUT_LIMITS_CMD:
 		status = _pid->SetOutputLimits(command->body);
 		break;

	case ENABLE_PWM:
		status = _pid->EnablePwm();
		break;

	case DISABLE_PWM:
		status = _pid->DisablePwm();
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
  pidConfig.adcChannel       		= ADC_CHANNEL; 			// Arduino ADC channel
  pidConfig.adcSampleRateHz  		= ADC_SAMPLE_RATE_HZ; 	// base clock rate for entire ADC, PWM and PID Loop
  pidConfig.adcDecimateLog2		 	= ADC_DECIMATE_LOG2;	// ADC updates 1 sample out of this many clock cycles
  pidConfig.pwmMode					= SW_PWM; 				// HW PDM not implemented
  pidConfig.pwmMinPulseWidth		= PWM_MIN_PULSE_WIDTH; 	// in units of ADC sample clock
  pidConfig.pwmNumBits 				= PWM_NUM_BITS;  		// period = PULSE_WIDTH * 2^(NUM_BITS)
  pidConfig.pwmPin 					= PWM_PIN;				// digital IO pin for PWM output
  pidConfig.loopUpdateRatio  		= PID_LOOP_UPDATE_RATIO;// PID updated 1 time out of this many clock cycles
  pidConfig.diagLedPin       		= PID_DIAG_LED_PIN; 	// digital IO pin for diagnostic LED output
  pidConfig.callback         		= &onNewSample; 		// Analog IF will call this method with a new ADC sample
  _pid 						 		= new PID( pidConfig ); // PID object configured as per above

  // Configure the Serial port to receive commands and send messages
  MSGConfig msgConfig;
  msgConfig.baudRate 		 = SERIAL_BAUD_RATE; 			// serial baud rate to communicate with host
  msgConfig.callback 		 = &onReceivedCommand; 			// Messager will call this method with new command from host
  _messager 				 = new Messager( msgConfig ); 	// messager instance

}

void loop() {

}
