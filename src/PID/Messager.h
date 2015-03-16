#ifndef Messager_h
#define Messager_h

#include "arduino.h"

#define STREAM_DELIMITER 0x3344

// MSG Types
#define	STATUS_MSG				0
#define ERROR_MSG				1
#define TEST_MSG				2

// CMD Types
#define LIMITS_CMD				10
#define RUN_CMD					11
#define SET_PWM_CMD				12
#define SET_LOOP_K_CMD			13
#define CONFIG_PWM_CMD			14
#define ENABLE_LOOP_CMD			15
#define ENABLE_STATUS_MSG_CMD	16
#define MAX_CMD_INDEX			16

// Error codes
#define CMD_TOO_LONG_ERROR		21
#define CMD_INVALID_CMD_ERROR	22
#define CMD_LOST_SYNC 			23
#define CMD_BAD_HEAD			24

// Length of the longest avaiable command, including
// everything except sync head/foot
// setLooConstantsCmd:
//  cmdLenght        = 2
//  cmd              = 1
//  timeStamp        = 4
//  7 floats = 7 x 4 = 28
//  1 int    = 1 x 2 = 2
//   TOTAL           = 37
#define MAX_CMD_LENGTH			37

// synch states
#define	SYNC_UNSYNC			 	0
#define SYNC_FIND_HEAD		 	1
#define SYNC_FIND_LENGTH	 	2
#define SYNC_FIND_CMDTYPE	 	3
#define SYNC_FIND_CMD	     	4
#define SYNC_FIND_TAIL	     	5
#define SYNC_WAIT_NEW_MSG    	6

typedef struct {
	uint8_t			errorCode;
} ErrorMsg;

typedef struct {
    uint16_t 	code;
	uint16_t 	value;
} TestMsg;


typedef struct {
	uint8_t 	cmdLength;
	uint8_t 	cmdType;
	uint32_t	timeStamp;
	uint8_t 	body[64];
} Command;


typedef struct MSGConfig {
  void (*callback)(Command *);
  uint32_t baudRate;
} MSGConfig;


typedef struct {
	uint16_t	head;
	uint16_t	length;
	uint8_t		msgType;
	uint32_t	timeStamp;
	uint8_t * 	body;
	uint16_t	tail;
} Frame;



// Command Structures

// 10
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
	float		value;
	bool		loopEnable;
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


class Messager
{

	public: Messager(MSGConfig);

	public:
		void sendMessage(uint8_t, uint8_t *, uint16_t ) ;
		void parseCommand();

	private:
		Frame 		_frame;
		MSGConfig 	_config;
};


#endif

