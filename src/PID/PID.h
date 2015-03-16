#ifndef PID_h
#define PID_h
#define LIB_VERSION	    0.0.1

#include "Sampler.h"

typedef unsigned long ulong;

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
    bool       enable;
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
    void      SetLoopConstants(int, int, int);
    void      UpdateLoop(int);
    void      SetOutputLimits(int, int);
    void      Enable(bool);
    void      EnableDataUpload( bool );
    int       GetKp();
    int       GetKi();
    int       GetKd();
    bool      GetMode();
    int       GetDirection();

  private:
    static void _adcCallbackWrapper(void *, int);
    byte        _serializePidState(char *);

  private:
    PIDConfig  _config;              // PID configuration structure passed to constructor
    bool       _ledState;
    PIDState   _pidState;
};
#endif


