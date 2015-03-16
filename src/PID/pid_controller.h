#ifndef PID_Controller_h
#define PID_Controller_h

#include "PID.h"
#include "Messager.h"

#define ENABLED	             1
#define DISABLED	     	 0

#define ADC_CHANNEL          1
#define ADC_SAMPLE_RATE_HZ   1
#define SERIAL_BAUD_RATE     500000
#define DAC_CHANNEL          0
#define USE_SW_PWM           1
#define PWM_PIN             12
#define PWM_PERIOD_SECONDS   2
#define PWM_NUM_BITS         8

#define PID_DIAG_LED_PIN     13
#define PID_LOOP_UPDATE_RATIO 1

#endif
