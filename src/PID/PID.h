#ifndef PID_h
#define PID_h
#define LIB_VERSION	    0.0.1

#include "AnalogIF.h"

typedef unsigned long ulong;

typedef struct {
	int16_t		kp;
	int16_t		ki;
	int16_t		kd;
	int16_t		kpAggressive;
	int16_t		kiAggressive;
	int16_t		kdAggressive;
	int16_t		aggressiveCutoffPoint;
	uint8_t 	loopUpdateRatio;
	int8_t 		loopPolarity;
} PIDLoopK;

typedef struct PIDState {
    int16_t      adcInput;
    int16_t      pidOutput;
    int16_t      setpoint;
    int16_t      dispKp;
    int16_t      dispKi;
    int16_t      dispKd;
    int16_t      kp;
    int16_t      ki;
    int16_t      kd;
    int16_t      ITerm;
    int16_t      DTerm;
    int16_t      lastInput;
    int16_t      outMin;
    int16_t      outMax;
    int8_t       controllerDirection;
    bool       enabled;
    bool	   outputUpdated;
    bool	   aggressiveMode;
} PIDState;

typedef struct PIDConfig {
  void 			(*callback)(PIDState *);
  byte 			adcChannel;
  uint16_t 		adcSampleRateHz;
  uint8_t 		adcDecimateLog2;
  byte 			loopUpdateRatio;
  pwm_t 		pwmMode;
  uint8_t       pwmMinPulseWidth;
  uint8_t	    pwmNumBits;
  uint8_t	    pwmPin;
  byte 			diagLedPin;
} PIDConfig;


class PID
{

  public:     PID(PIDConfig);

  public:

  	int8_t	  	EnableSampler( void );
  	int8_t	  	DisableSampler( void );
    int8_t	  	ConfigurePwm(uint8_t *);
    int8_t    	SetLoopConstants( uint8_t * );
    PIDLoopK*   GetLoopConstants( void );
    int8_t    	SetOutputLimits( uint8_t * );
    int8_t		EnablePwm( void );
    int8_t 		DisablePwm( void );
    int8_t	  	SetPwm( uint8_t * );
    int8_t	  	SetSetpoint( uint8_t * );
    int8_t      EnableLoop( void );
    int8_t      DisableLoop( void );
    int8_t      EnableStatusMessages( void );
    int8_t      DisableStatusMessages( void );

  private:
    static void _adcCallbackWrapper( void *, int );
    void        _updateLoop( int );
    PwmConfig	_pwmConfig;

  private:
    AnalogIF	*_analogIF;
    PIDConfig  	 _config;     		// PID configuration structure passed to constructor
    PIDState   	 _pidState;   		// PID private data structure maintaining current state of PID
    PIDLoopK 	 _pidLoopK;			// PID loop constants, copied into PIDState
    uint8_t    	 _ledState;			// private variable to toggle diagnostics LED on and off
    bool	     _statusMsgEnabled;
};
#endif


