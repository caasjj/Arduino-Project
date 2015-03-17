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
	uint8_t 	loopPolarity;
} loopConstants;

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
} PIDState;

typedef struct PIDConfig {
  void (*callback)(PIDState *);
  byte adcChannel;
  int adcSampleRateHz;
  byte loopUpdateRatio;
  byte diagLedPin;
} PIDConfig;



class PID
{

  public:     PID(PIDConfig);

  public:

  	void	  EnableSampler( void );
  	void	  DisableSampler( void );
    bool	  ConfigurePwm(uint8_t *);
    bool      SetLoopConstants( uint8_t * );
    bool      SetOutputLimits( uint8_t * );
    bool	  SetPwm( uint8_t * );
    bool	  SetSetpoint( uint8_t * );
    void      EnableLoop( void );
    void      DisableLoop( void );
    void      EnableStatusMessages( void );
    void      DisableStatusMessages( void );

  private:
    static void _adcCallbackWrapper( void *, int );
    void        _updateLoop( int );

  private:
    PIDConfig  _config;     		// PID configuration structure passed to constructor
    PIDState   _pidState;   		// private data structure maintaining current state of PID
    bool       _ledState;			// private variable to toggle diagnostics LED on and off
    bool	   _statusMsgEnabled;
};
#endif


