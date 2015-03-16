#ifndef Sampler2_h
#define Sampler2_h

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

void Sampler_setup(char, int, void *, void(*)(void *, int));

#endif
