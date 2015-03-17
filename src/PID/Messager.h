#ifndef Messager_h
#define Messager_h

#include "arduino.h"

#define FRAME_DELIMITER 0x3344
#define FRAME_HEADER_BYTES	2*sizeof(uint16_t)+sizeof(uint32_t)+sizeof(msg_t)

// MSG Types
#define STATUS_BASE_OFFSET	0
typedef enum msg_t {						\
 STATUS_MSG,								\
 ERROR_MSG,									\
 TEST_MSG,									\
 ACK_MSG									\
} msg_t;									\

// CMD Types
#define CMD_BASE_OFFSET		10
typedef enum {								\
 ENABLE_SAMPLER_CMD = CMD_BASE_OFFSET,  	\
 DISABLE_SAMPLER_CMD,  						\
 CONFIGURE_PWM_CMD,							\
 SET_LOOP_K_CMD,							\
 GET_LOOP_K_CMD,							\
 SET_OUTPUT_LIMITS_CMD,						\
 SET_PWM_CMD,								\
 SET_SETPOINT_CMD,							\
 ENABLE_LOOP_CMD,							\
 DISABLE_LOOP_CMD,							\
 ENABLE_STATUS_MESSAGES_CMD,				\
 DISABLE_STATUS_MESSAGES_CMD,				\
 INVALID_CMD								\
} cmd_t;

// Command parsing Error codes
#define CMD_ERR_BASE_OFFSET 20

typedef enum {								\
 TOO_LONG_CMD_ERR = CMD_ERR_BASE_OFFSET,	\
 INVALID_CMD_ERR,							\
 LOST_SYNC_CMD_ERR,							\
 BAD_HEAD_CMD_ERR,							\
 UNKNOWN_CMD_ERR 							\
} cmd_err_t;

// Length of the longest avaiable command, including
// everything except sync head/foot
// setLoopConstantsCmd:
//  cmdLenght        = 2
//  cmd              = 1
//  timeStamp        = 4
//  7 floats = 7 x 4 = 28
//  1 int    = 1 x 2 = 2
//   TOTAL           = 37
#define MAX_CMD_LENGTH			37

// synch states
#define SYNC_STATE_BASE_OFFSET 	5

typedef enum {								\
 SYNC_UNSYNC = SYNC_STATE_BASE_OFFSET,		\
 SYNC_FIND_HEAD,							\
 SYNC_FIND_LENGTH,							\
 SYNC_FIND_CMD,								\
 SYNC_FIND_TAIL,							\
} sync_state_t;

typedef struct {
	cmd_err_t	errorCode;
} ErrorMsg;

typedef struct {
	sync_state_t 	state;
    uint16_t 		code;
	uint16_t 		value;
} TestMsg;

typedef struct {
	int8_t	status;
	cmd_t	cmdType;
} AckMsg;


typedef struct {
	uint8_t 	cmdLength;
	cmd_t 		cmdType;
	uint32_t	timeStamp;
	uint8_t 	body[64];
} Command;


typedef struct MSGConfig {
  int8_t (*callback)(Command *);
  uint32_t baudRate;
} MSGConfig;


typedef struct {
	uint16_t	headDelimiter;
	uint16_t	length;
	msg_t		msgType;
	uint32_t	timeStamp;
	uint8_t * 	body;
	uint16_t	tailDelimiter;
} Frame;

class Messager
{

	public: Messager(MSGConfig);

	public:
		void 		sendMessage(msg_t, uint8_t *, uint16_t ) ;
		void 		parseCommand();

	private:
		Frame 		_frame;
		MSGConfig 	_config;
};


#endif

