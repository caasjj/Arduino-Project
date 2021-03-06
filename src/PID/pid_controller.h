#ifndef PID_Controller_h
#define PID_Controller_h

#include "PID.h"
#include "Messager.h"

#define ADC_CHANNEL          	1
#define ADC_SAMPLE_RATE_HZ   	64
#define ADC_DECIMATE_LOG2 	 	4
#define SERIAL_BAUD_RATE     	500000
#define PWM_PIN             	12
#define PWM_MIN_PULSE_WIDTH  	6
#define PWM_NUM_BITS         	5

#define PID_DIAG_LED_PIN     	13
#define PID_LOOP_UPDATE_RATIO   4

//Command Structures

typedef struct {
	bool 		enable;
} EnableSamplerCmd;

//10
typedef struct {
	float		maxOutput;
	float		minOutput;
} ConfigLimitsCmd;

// 11
typedef struct {

} SetSetpointCmd;

// 12
typedef struct {
	float		value;
	bool		loopEnable;
} SetPwmCmd;

// 13
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
} SetLoopConstantsCmd;

// 14
typedef struct {

} ConfigPwmCmd;

// 15
typedef struct {

} EnableLoopCmd;

// 16
typedef struct {
	bool	enabled;
} EnableStatusMsgCmd;


#endif
