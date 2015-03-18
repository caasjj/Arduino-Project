#ifndef PID_h
#define PID_h
#define LIB_VERSION	    0.0.1

#include "Sampler.h"

typedef unsigned long ulong;

typedef struct {
	float		kp;
	float		ki;
	float		kd;
	float		kpAggressive;
	float		kiAggressive;
	float		kdAggressive;
	float		aggressiveCutoffPoint;
	uint8_t 	loopUpdateRatio;
	int8_t 		loopPolarity;
} PIDLoopK;

typedef struct PIDState {
    float      adcInput;
    float      pidOutput;
    float      setpoint;
    float      dispKp;
    float      dispKi;
    float      dispKd;
    float      kp;
    float      ki;
    float      kd;
    float      ITerm;
    float      DTerm;
    float      lastInput;
    float      outMin;
    float      outMax;
    bool       controllerDirection;
    bool       enabled;
    bool	   outputUpdated;
    bool	   aggressiveMode;
} PIDState;

typedef struct PIDConfig {
  void 			(*callback)(PIDState *);
  byte 			adcChannel;
  int 			adcSampleRateHz;
  uint8_t 		samplesToAverage;
  byte 			loopUpdateRatio;
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
    int8_t	  	SetPwm( uint8_t * );
    int8_t	  	SetSetpoint( uint8_t * );
    int8_t      EnableLoop( void );
    int8_t      DisableLoop( void );
    int8_t      EnableStatusMessages( void );
    int8_t      DisableStatusMessages( void );

  private:
    static void _adcCallbackWrapper( void *, int );
    void        _updateLoop( int );

  private:
    PIDConfig  	 _config;     		// PID configuration structure passed to constructor
    PIDState   	 _pidState;   		// PID private data structure maintaining current state of PID
    PIDLoopK 	 _pidLoopK;			// PID loop constants, copied into PIDState
    uint8_t    	 _ledState;			// private variable to toggle diagnostics LED on and off
    bool	     _statusMsgEnabled;
};
#endif


