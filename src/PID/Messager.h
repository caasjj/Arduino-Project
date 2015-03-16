#ifndef Messager_h
#define Messager_h

#include "arduino.h"

#define STREAM_DELIMITER 0x3344

typedef struct Frame {
	unsigned int	head;
	unsigned int	length;
	byte			msgType;
	unsigned long	timeStamp;
	byte * 			body;
	unsigned int	tail;
} Frame;

class Messager
{

	private:
		Frame _frame;

	public: Messager(unsigned long);

	public:
		void sendMessage(byte, byte *, unsigned int ) ;

};


#endif

