#include "Messager.h"

Messager::Messager(unsigned long baudRate) {

	  Serial.begin( baudRate );
	  _frame.head = STREAM_DELIMITER;

}

void Messager::sendMessage(byte msgType, byte *buffer, unsigned int length) {

	  // prepare frame for new message
	  _frame.length    = length + 7;
	  _frame.msgType   = msgType;
	  _frame.timeStamp = millis();
	  _frame.body      = buffer;

	  unsigned int count = 0;
	  // walking pointer
	  byte *ptr;

	  // Write the header, length and type
	  ptr = (byte *) &_frame.head;
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
	  ptr = (byte *) &_frame.head;
	  Serial.write(~(*ptr++));
	  Serial.write(~(*ptr++));

	  // make sure all is written out
	  Serial.flush();

}
