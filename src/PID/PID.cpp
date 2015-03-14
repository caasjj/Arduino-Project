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
  _pidState.pidOutput = 0.0;
  
  pinMode(_config.diagLedPin, OUTPUT);
  digitalWrite(_config.diagLedPin, LOW);  
  
  
  Sampler_setup(_config.adcChannel, _config.adcSampleRateHz, this, PID::_adcCallbackWrapper); 

}
 
/********************************************************************************************* 
 *
 * Class wrapper to allow ADC ISR to call back the UpdateLoop method of PID object
 * passed in pidPtr. This is NOT the callback for the controller that instantiated the 
 * PID object, but the callback PID gives to the Sampler to be called on every sample.
 *
 *********************************************************************************************/ 
void PID::_adcCallbackWrapper(void *pidPtr, int adcValue) {
  PID* mySelf = (PID*) pidPtr;
  mySelf->UpdateLoop(adcValue);
}

/********************************************************************************************* 
 *
 * SetLoopConstants
 *
 *********************************************************************************************/ 
void PID::SetLoopConstants(int Kp, int Ki, int Kd)
{
   
}

/********************************************************************************************* 
 *
 * UpdateLoop
 *
 *********************************************************************************************/ 
void PID::UpdateLoop(int adcValue)
{
  _ledState = !_ledState;
  digitalWrite(_config.diagLedPin, _ledState);
  
  _pidState.timeStamp = millis();
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
  _pidState.enable = true;
  
  // callback the PID instantiator and give it the data
    _config.callback(&_pidState);

}
  
/********************************************************************************************* 
 *
 * SetOutputLimits
 *
 *********************************************************************************************/ 
void PID::SetOutputLimits(int Min, int Max)
{
   
}

/********************************************************************************************* 
 *
 * Enable
 *
 *********************************************************************************************/ 
void PID::Enable(bool enabled)
{
    
}

 /********************************************************************************************* 
 *
 * EnableData
 *
 *********************************************************************************************/ 
void PID::EnableDataUpload( bool enabled )
{
    
}

/********************************************************************************************* 
 *
 * Getters - DEBUG only!
 *
 *********************************************************************************************/ 
int PID::GetKp(){ return  _pidState.dispKp; }
int PID::GetKi(){ return  _pidState.dispKi;}
int PID::GetKd(){ return  _pidState.dispKd;}

bool PID::GetMode(){ return  _pidState.enable; }
int PID::GetDirection(){ return _pidState.controllerDirection;}









