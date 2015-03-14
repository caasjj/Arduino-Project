/**********************************************************************************************
 * Arduino ADC Sampler Library - Version 0.0.1
 * by Walid Hosseini <caasjj@gmail.com> walidhosseini.com
 *
 * Uses the selected timer to sample the selected ADC channel at the selected smaple rate.
 * Data is provided to the caller via callback, thereby decoupling this class from the caller.
 *
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/

#include "Sampler2.h"
#include "PID.h"

void (* lCallback)(int);
void (* lCallback2)(void *, int);
void *lObj;


/********************************************************************************************* 
 *
 * Sampler Constructure
 *
 *********************************************************************************************/ 
void Sampler_setup(char adcChannel, int rateInHz, void *Obj, void (*callback)(void* Obj, int)) 
{
    cli();
    
    lCallback2 = callback;
    lObj = Obj;
    
    timer_setup(rateInHz);
  
    // set up ADC
    ADMUX  = B01000000 | (0xF & adcChannel);
    ADCSRB = B00000000;
  
    timer_start();

    sei();
}

ISR(TIMER_VECTOR) 
{
    ADCSRA = B11001111;
}

ISR(ADC_vect) 
{
    unsigned char adcl = ADCL;
    unsigned char adch = ADCH;
    unsigned int adcVal = (adch << 8) | adcl;
    lCallback2( lObj, adcVal );
}
