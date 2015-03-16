#include "Messager.h"

Messager 	*__myself;

uint8_t 	__syncState;
bool		__synched;
Command		__command;

Messager::Messager(MSGConfig config) {

	  _config = config;

	  Serial.begin( _config.baudRate );
	  _frame.head = STREAM_DELIMITER;

	  __myself = this;
	  __syncState = SYNC_UNSYNC;
	  __synched = false;
}

void Messager::sendMessage(uint8_t msgType, uint8_t *buffer, uint16_t length) {

	  // prepare frame for new message
	  _frame.length    = length + 7;
	  _frame.msgType   = msgType;
	  _frame.timeStamp = millis();
	  _frame.body      = buffer;

	  uint16_t count = 0;
	  // walking pointer
	  uint8_t *ptr;

	  // Write the header, length and type
	  ptr = (uint8_t *) &_frame.head;
	  while (count++ < 9u) {
		Serial.write(*ptr++);
	  }

	  // write tye body
	  count = 0;
	  while(count++ < length) {
		Serial.write(*buffer++);
		ptr++;
	  }

	  // write the trailer
	  ptr = (uint8_t *) &_frame.head;
	  Serial.write(~(*ptr++));
	  Serial.write(~(*ptr++));

	  // make sure all is written out
	  Serial.flush();

}

void Messager::parseCommand() {

	TestMsg testMsg;

	_config.callback( &__command );

}

void serialEvent() {

	uint8_t 	lo, hi, errorCode, cmdType;
	uint16_t 	cmdLength, head, tail;

	int 		numBytes;

	ErrorMsg errorMsg;
	TestMsg	 testMsg;

	numBytes = Serial.available();

	switch (__syncState) {

		case	SYNC_UNSYNC:

			if (numBytes < 2) break;

			while(__syncState == SYNC_UNSYNC && numBytes > 0) {
				while( numBytes-- > 0 && Serial.read() != 0x44 );
				if (numBytes > 0 && Serial.peek()==0x33) {
					Serial.read();
					__syncState = SYNC_FIND_LENGTH;
				}
			}
			break;

        case SYNC_FIND_HEAD:
        	if (numBytes < 2) break;

        	lo = Serial.read();
        	hi = Serial.read();
        	head = hi<<8 | lo;
        	if( head != STREAM_DELIMITER) {
				__syncState = SYNC_UNSYNC;
				errorMsg.errorCode = CMD_BAD_HEAD;
				__myself->sendMessage(ERROR_MSG, (byte*) &errorMsg, (uint16_t) sizeof(errorMsg) );
        	}
        	else {
        		__syncState = SYNC_FIND_LENGTH;
        	}
			break;

        case SYNC_FIND_LENGTH:
            if (numBytes < 2) break;
			lo = Serial.read();
			hi = Serial.read();
			cmdLength = hi << 8 | lo;
			if (cmdLength > MAX_CMD_LENGTH) {
				errorMsg.errorCode = CMD_TOO_LONG_ERROR;
				__myself->sendMessage(ERROR_MSG, (byte*) &errorMsg, (uint16_t) sizeof(errorMsg) );
				__syncState = SYNC_UNSYNC;

			} else {
             	__command.cmdLength = cmdLength;
             	__syncState = SYNC_FIND_CMD;

             }
			break;

        case SYNC_FIND_CMD:
			if (Serial.available() < __command.cmdLength - 2) return; // already read the length
			cmdType = Serial.read();
			if (cmdType > MAX_CMD_INDEX) {
				errorMsg.errorCode = CMD_INVALID_CMD_ERROR;
				__myself->sendMessage(ERROR_MSG, (byte*) &errorMsg, (uint16_t) sizeof(errorMsg) );
				__syncState = SYNC_UNSYNC;
			}
			else
			{
				__command.cmdType = cmdType;
				uint8_t *ptr;
				ptr = (uint8_t *) &__command.timeStamp;
				for(uint8_t i=0; i<__command.cmdLength - 3; i++) {
					*ptr++ = Serial.read();
				}
				__syncState = SYNC_FIND_TAIL;
			}
			break;

        case SYNC_FIND_TAIL:
			if (numBytes < 2) break;
			lo = Serial.read();
			hi = Serial.read();
			tail = hi<<8 | lo;
			if( tail != (uint16_t) ~STREAM_DELIMITER) {
				__syncState = SYNC_UNSYNC;

				errorMsg.errorCode = CMD_LOST_SYNC;
				__myself->sendMessage(ERROR_MSG, (byte*) &errorMsg, (uint16_t) sizeof(errorMsg) );

			}
			else {
				__syncState = SYNC_FIND_HEAD;
				__myself->parseCommand();
			}
			break;

        default: break;
	}

}
