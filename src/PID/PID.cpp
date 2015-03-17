/**********************************************************************************************
 * Arduino PID Library - Version 0.0.1
 * by Walid Hosseini <caasjj@gmail.com> walidhosseini.com
 *
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/
#include "Arduino.h"
#include "PID.h"

/*********************************************************************************************
 *
 * PID Constructure
 *
 *********************************************************************************************/
PID::PID(PIDConfig config)
{
  _config = config;

  pinMode(_config.diagLedPin, OUTPUT);
  digitalWrite(_config.diagLedPin, LOW);

  Sampler_setup(_config.adcChannel, _config.adcSampleRateHz, this, PID::_adcCallbackWrapper);

  // Start off in disabled mode
  _statusMsgEnabled 	= false;
  _pidState.enabled 	= false;
  _pidState.pidOutput  	= 0.0;


  // TEMPORARU
  //Sampler_start();
}


/*********************************************************************************************
 *
 * disable sampler
 *
 *********************************************************************************************/
void PID::EnableSampler(void)
{
	Sampler_start();
}


/*********************************************************************************************
 *
 * disable sampler
 *
 *********************************************************************************************/
void PID::DisableSampler(void)
{

	Sampler_stop();

}

/*********************************************************************************************
 *
 * ConfigurePwm
 *
 *********************************************************************************************/
bool PID::ConfigurePwm(uint8_t *data)
{
  return (*data>0);
}

/*********************************************************************************************
 *
 * SetLoopConstants
 *
 *********************************************************************************************/
bool PID::SetLoopConstants(uint8_t *data)
{
  return (*data>0);
}


/*********************************************************************************************
 *
 * SetOutputLimits
 *
 *********************************************************************************************/
bool PID::SetOutputLimits(uint8_t *data)
{
  return (*data>0);
}

/*********************************************************************************************
 *
 * SetPwm
 *
 *********************************************************************************************/
bool PID::SetPwm(uint8_t *data)
{
  return (*data>0);
}

/*********************************************************************************************
 *
 * SetSetPoint
 *
 *********************************************************************************************/
bool PID::SetSetpoint(uint8_t *data)
{
  return (*data>0);
}

/*********************************************************************************************
 *
 * EnableLoop
 *
 *********************************************************************************************/
void PID::EnableLoop( void )
{
	_pidState.enabled = true;
}

/*********************************************************************************************
 *
 * DisableLoop
 *
 *********************************************************************************************/
void PID::DisableLoop( void )
{
	_pidState.enabled = false;
}

 /*********************************************************************************************
 *
 * EnableStatusMessages
 *
 *********************************************************************************************/
void PID::EnableStatusMessages( void )
{
	_statusMsgEnabled = true;
}

/*********************************************************************************************
 *
 * DisableStatusMessages
 *
 *********************************************************************************************/
void PID::DisableStatusMessages( void )
{
	_statusMsgEnabled = false;
}

/*********************************************************************************************
 *
 * Class wrapper to allow ADC ISR to call back the UpdateLoop method of PID object
 * passed in pidPtr. This is NOT the callback for the controller that instantiated the
 * PID object, but the callback PID gives to the Sampler to be called on every sample.
 *
 *********************************************************************************************/
void PID::_adcCallbackWrapper(void *pidPtr, int adcValue) {
  //PID* mySelf = (PID*) pidPtr;
  ((PID*) pidPtr)->_updateLoop(adcValue);
}

/*********************************************************************************************
 *
 * UpdateLoop
 *
 *********************************************************************************************/
void PID::_updateLoop(int adcValue)
{
  _ledState = !_ledState;
  digitalWrite(_config.diagLedPin, _ledState);

  _pidState.adcInput = adcValue;
  _pidState.pidOutput = _pidState.pidOutput + 1.0;
  _pidState.setpoint = 9.0;
  _pidState.dispKp = 0.12;
  _pidState.dispKi = 0.98;
  _pidState.dispKd = -1.24;
  _pidState.kp = 12;
  _pidState.ki = 15;
  _pidState.kd = -123;
  _pidState.ITerm = 8997.8;
  _pidState.DTerm = -232.11;
  _pidState.lastInput = 121;
  _pidState.outMin = -1000;
  _pidState.outMax = 1000;
  _pidState.controllerDirection = 1;
  _pidState.enabled = true;
  _pidState.outputUpdated = false;

  // callback the PID instantiator and give it the data
    _config.callback(&_pidState);

}









