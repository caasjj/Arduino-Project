#include "Messager.h"

Messager 	*__myself;

sync_state_t 	__syncState;
Command			__command;

Messager::Messager(MSGConfig config) {

	  _config = config;

	  Serial.begin( _config.baudRate );
	  _frame.headDelimiter = FRAME_DELIMITER;

	  __myself = this;
	  __syncState = SYNC_UNSYNC;
}

void Messager::sendMessage(msg_t msgType, uint8_t *buffer, uint16_t length) {

	  // prepare frame for new message

	  // length is the message length + msgType (msg_t enum) + msgLength (uint16) + timeStamp (uint32)
	  _frame.length    = length + sizeof(msg_t) + sizeof(uint16_t) + sizeof(uint32_t);
	  _frame.msgType   = msgType;
	  _frame.timeStamp = millis();
	  _frame.body      = buffer;

	  uint16_t count = 0;
	  // walking pointer
	  uint8_t *ptr;

	  // Write the header, length and type
	  ptr = (uint8_t *) &_frame.headDelimiter;
	  while (count++ < FRAME_HEADER_BYTES) {
		Serial.write(*ptr++);
	  }

	  // write tye body
	  count = 0;
	  while(count++ < length) {
		Serial.write(*buffer++);
		ptr++;
	  }

	  // write the trailer
	  ptr = (uint8_t *) &_frame.headDelimiter;
	  Serial.write(~(*ptr++));
	  Serial.write(~(*ptr++));

	  // make sure all is written out
	  Serial.flush();

}

void Messager::parseCommand() {

	AckMsg ack;

	ack.status = _config.callback( &__command );
	ack.cmdType = __command.cmdType;

	if (ack.status != 0)
		sendMessage(ACK_MSG, (byte *)& ack, (uint16_t)sizeof(ack));

}

void serialEvent() {

	uint8_t 	lo, hi;
	cmd_t 		cmdType;
	uint16_t 	cmdLength, head, tail;
	uint16_t 	numBytes;

	ErrorMsg 	errorMsg;

	numBytes = Serial.available();

	switch (__syncState) {

		case	SYNC_UNSYNC:

			if (numBytes < sizeof(uint16_t)) break;

			while(__syncState == SYNC_UNSYNC && numBytes > 0) {
				while( numBytes-- > 0 && Serial.read() != (FRAME_DELIMITER & 0xFF) );
				if (numBytes > 0 && Serial.peek()== ( (FRAME_DELIMITER>>8) & 0xFF) ) {
					Serial.read();
					__syncState = SYNC_FIND_LENGTH;
				}
			}
			break;

        case SYNC_FIND_HEAD:

        	if (numBytes < sizeof(FRAME_DELIMITER)) break;

        	lo = Serial.read();
        	hi = Serial.read();
        	head = hi<<8 | lo;

        	if( head != FRAME_DELIMITER) {
				__syncState = SYNC_UNSYNC;

				errorMsg.errorCode = BAD_HEAD_CMD_ERR;
				__myself->sendMessage(ERROR_MSG, (byte*) &errorMsg, (uint16_t) sizeof(errorMsg) );
        	}
        	else {
        		__syncState = SYNC_FIND_LENGTH;
        	}
			break;

        case SYNC_FIND_LENGTH:
            if (numBytes < sizeof(uint16_t)) break;

			lo = Serial.read();
			hi = Serial.read();
			cmdLength = hi << 8 | lo;

			if (cmdLength > MAX_CMD_LENGTH) {

				errorMsg.errorCode = TOO_LONG_CMD_ERR;
				__myself->sendMessage(ERROR_MSG, (byte*) &errorMsg, (uint16_t) sizeof(errorMsg) );
				__syncState = SYNC_UNSYNC;

			} else {
             	__command.cmdLength = cmdLength;
             	__syncState = SYNC_FIND_CMD;
             }
			break;

        case SYNC_FIND_CMD:
			if (numBytes < __command.cmdLength - sizeof(uint16_t)) return; // already read the length

			lo = Serial.read();
			hi = Serial.read();
			cmdType = (cmd_t) (hi<<8 | lo);

			// WARNING: counting on integer enum!!
			if (cmdType >= INVALID_CMD) {
				errorMsg.errorCode = INVALID_CMD_ERR;
				__myself->sendMessage(ERROR_MSG, (byte*) &errorMsg, (uint16_t) sizeof(errorMsg) );
				__syncState = SYNC_UNSYNC;
			}
			else
			{
				__command.cmdType = cmdType;

				uint8_t *ptr;
				ptr = (uint8_t *) &__command.timeStamp;
				// cmdLength =
				//    2 (command length field, uint16_t) +
				//    N (length of body)       +
				//    2 (length of footer delimiter, uint16_t)
				// So to read N, the body length, we need
				// cmdLength - sizeof(uint16_t) - length of delimiter
				for(uint8_t i=0; i < __command.cmdLength - (sizeof(FRAME_DELIMITER)+sizeof(uint16_t)); i++) {
					*ptr++ = Serial.read();
				}
				__syncState = SYNC_FIND_TAIL;
			}
			break;

        case SYNC_FIND_TAIL:
			if (numBytes < sizeof(FRAME_DELIMITER)) break;

			lo = Serial.read();
			hi = Serial.read();
			tail = hi<<8 | lo;

			if( tail != (uint16_t) ~FRAME_DELIMITER) {
				__syncState = SYNC_UNSYNC;

				errorMsg.errorCode = LOST_SYNC_CMD_ERR;
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
