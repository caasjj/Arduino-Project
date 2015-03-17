/**********************************************************************************************
 * Arduino ADC Sampler Library - Version 0.0.1
 * by Walid Hosseini <caasjj@gmail.com> walidhosseini.com
 *
 * Uses the selected timer to sample the selected ADC channel at the selected smaple rate.
 * Data is provided to the caller via callback, thereby decoupling this class from the caller.
 *
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/

#include "Sampler.h"
#include "PID.h"

void (* lCallback)(void *, int);
void *lObj;


/*********************************************************************************************
 *
 * Sampler Constructure
 *
 *********************************************************************************************/
void Sampler_setup(char adcChannel, int rateInHz, void *Obj, void (*callback)(void* Obj, int))
{
	// disable interrupts
    cli();

    lCallback = callback;
    lObj = Obj;

	// macro to set up sampler timer
    timer_setup(rateInHz);

    // set up ADC
    ADMUX  = B01000000 | (0xF & adcChannel);
    ADCSRB = B00000000;

	// macro to enable TIMSK
    //timer_start();

	// re-enable iterrupts
    sei();
}

void Sampler_start() {

	cli();
	timer_start();
	sei();
}

void Sampler_stop() {
	cli();
	timer_stop();
	sei();
}

// TIMER_VECTOR initialized TIMERx_COMPA_vect, where x is the
// selected timer (0, 1, or 2) in TIMER constant in Sampler.h
ISR(TIMER_VECTOR)
{
    ADCSRA = B11001111;
}

ISR(ADC_vect)
{
    unsigned char adcl = ADCL;
    unsigned char adch = ADCH;
    unsigned int adcVal = (adch << 8) | adcl;
    lCallback( lObj, adcVal );
}
