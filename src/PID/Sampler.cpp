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

void (* __Callback)(void *, int);
void *lObj;

// ADC local variables
uint16_t 	__AdcAccumulator;
uint8_t 	__AdcCounter = 0;
uint8_t 	__AdcDecimateLog2 = 0;

// PWM local variables
uint8_t		__PwmValue;
uint8_t		__PwmPin;
uint8_t		__PwmNumbits;
uint8_t		__PwmDecimateLog2;
/*********************************************************************************************
 *
 * Sampler Constructure
 *
 *********************************************************************************************/
void Sampler_setup(char adcChannel, int rateInHz, uint8_t decimateLog2, void *Obj, void (*callback)(void* Obj, int))
{
	// disable interrupts
    cli();

    __Callback = callback;
    lObj = Obj;
    __AdcDecimateLog2 = decimateLog2;

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

	// Decimate, average and update ADC value to caller
    if (__AdcCounter++ == ( 1 << __AdcDecimateLog2) ) {
    	__Callback( lObj, __AdcAccumulator >> __AdcDecimateLog2 );
    	__AdcCounter 	 = 0;
    	__AdcAccumulator = 0;
    } else {
    	__AdcAccumulator += adcVal;
    }

	// Update PWM state based on __PwmValue
}

void Pwm_Setup(uint8_t pin, uint8_t decimateLog2, uint8_t numbits) {

	__PwmPin = pin;
    __PwmNumbits =  numbits;
    __PwmDecimateLog2 = decimateLog2;

	pinMode( __PwmPin, OUTPUT );
	digitalWrite( __PwmPin, LOW);
}

void Pwm_update(uint8_t value) {

	__PwmValue = value;
}
