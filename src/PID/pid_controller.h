#ifndef PID_Controller_h
#define PID_Controller_h

#include "PID.h"
#include "Messager.h"
#include "pid_msgTypes.h"

#define ENABLED	             1
#define DISABLED	     	 0

#define DIAG_LED_PIN        13
#define ADC_CHANNEL          1
#define ADC_SAMPLE_RATE_HZ   1
#define SERIAL_BAUD_RATE     500000
#define DAC_CHANNEL          0
#define USE_SW_PWM           1
#define PWM_PIN             12
#define PWM_PERIOD_SECONDS   2
#define PWM_NUM_BITS         8

void sendFrame(byte, void *, unsigned int);


#endif
