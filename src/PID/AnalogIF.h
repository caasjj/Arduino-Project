#ifndef AnalogIF_H
#define AnalogIF_H

#define LIB_VERSION	0.0.1

#include "Arduino.h"

#ifndef TIMER
  #define TIMER 1
#endif

#if TIMER == 0
  #define TCCRA TCCR0A
  #define TCCRB TCCR0B
  #define TCNT  TCNT0
  #define OCRA  OCR0A
  #define TIMSK TIMSK0
  #define OCIEA OCIE0A
  #define CTCEN TCCR0A
  #define WGM   WGM01
  #define TIMER_VECTOR TIMER0_COMPA_vect
  #define timer_setup(freqHz) {                   \
    TCCRA      = 0;                               \
    TCCRB      = 0;                               \
    TCNT       = 0;                               \
    OCRA       = 15625 / freqHz - 1;              \
    CTCEN     |= (1 << WGM );                     \
    TCCRB     |= 5;                               \
  }
#elif TIMER == 1
  #define TCCRA TCCR1A
  #define TCCRB TCCR1B
  #define TCNT  TCNT1
  #define OCRA  OCR1A
  #define TIMSK TIMSK1
  #define OCIEA OCIE1A
  #define CTCEN TCCR1B
  #define WGM   WGM12
  #define TIMER_VECTOR TIMER1_COMPA_vect
  #define timer_setup(freqHz) {                   \
    TCCRA      = 0;                               \
    TCCRB      = 0;                               \
    TCNT       = 0;                               \
    OCRA       = (long) ( 15625 / freqHz - 1 ) ;  \
    CTCEN     |= (1 << WGM );                     \
    TCCRB     |= 5;                               \
  }
#else
  #define TCCRA TCCR2A
  #define TCCRB TCCR2B
  #define TCNT  TCNT2
  #define OCRA  OCR2A
  #define TIMSK TIMSK2
  #define OCIEA OCIE2A
  #define CTCEN TCCR2A
  #define WGM   WGM21
  #define TIMER_VECTOR TIMER2_COMPA_vect
  #define timer_setup(freqHz) {                   \
    TCCRA      = 0;                               \
    TCCRB      = 0;                               \
    TCNT       = 0;                               \
    OCRA       = 15625 / freqHz - 1;              \
    CTCEN     |= (1 << WGM );                     \
    TCCRB     |= 7;                               \
  }
#endif

#define timer_start() {                           \
    TIMSK |= (1 << OCIEA);                        \
}

#define timer_stop() {                            \
    TIMSK &= ~(1 << OCIEA);                       \
}

typedef enum {
	HW_PWM,				\
	SW_PWM 				\
} pwm_t;

typedef struct {
  	pwm_t	     pwmMode;
	uint8_t      pwmMinPulseWidth;
  	uint8_t	     pwmNumBits;
	uint8_t	     pwmPin;
} PwmConfig;


class AnalogIF
{

	// constructor
	public:
        AnalogIF(uint16_t, uint8_t, uint8_t, void *, void (*)(void *, int));

	// public methods
	public:
		void	      startAdc( void );
		void 	      stopAdc( void );
		void 	      useAdcChannel( uint8_t );
		bool          configPwm( PwmConfig );
		void          enableSwPwm( void );
		void          disableSwPwm( void );
		void          setPwmValue( uint16_t pwmValue );
		void          updateAdcSinc( unsigned int );
		void          updateSwPwm( void );

	// private functions
	private:

        void          ( *_callback )( void *, int );
		void		  *_context;

	// private state
	private:
        uint8_t       _fundamentalRateHz;

		uint8_t       _adcChannel;
		uint16_t      _adcAccumulator;
		uint16_t      _adcCounter;
		uint8_t       _adcDecimateLog2;
		uint16_t      _pwmValue;
		bool          _swPwmEnable;
		uint8_t       _swPwmPin;
		uint8_t       _swPwmMinPulseWidth;
		uint8_t       _swPwmNumBits;
        uint16_t      _swPwmValueCounter;
        uint16_t      _swPwmClockCounter;
        uint16_t      _swPwmMaxCount;
};

#endif
