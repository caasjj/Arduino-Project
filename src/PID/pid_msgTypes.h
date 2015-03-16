#ifndef PID_MsgTypes_H
#define PID_MsgTypes_H

#include "arduino.h"
#include "PID.h"

// MSG Types
#define	STATUS_MSG			0
#define ERROR_MSG			1
#define TEST_MSG			2

// CMD Types
#define CMD_TYPE_CONFIG			10
#define CMD_TYPE_LIMITS			11
#define CMD_TYPE_RUN			12
#define CMD_TYPE_SET_PWM		13
#define CMD_TYPE_SET_LOOP_K		14
#define CMD_TYPE_CONFIG_PWD		15

typedef struct StatusMsg {
	PIDState		*pidState;
} statusMsg;

typedef struct ErrorMsg {
	byte			errorCode;
} ErrorMsg;

typedef struct TestMsg {
    unsigned int size;
	unsigned int count;
} TestMsg;

#endif
