/**********************************************************************************************
 * Arduino ADC Sampler Library - Version 0.0.1
 * by Walid Hosseini <caasjj@gmail.com> walidhosseini.com
 *
 * Uses the selected timer to sample the selected ADC channel at the selected smaple rate.
 * Data is provided to the caller via callback, thereby decoupling this class from the caller.
 *
 * This Library is licensed under a GPLv3 License
 **********************************************************************************************/
#include "AnalogIF.h"

AnalogIF    *__analogIFcbObjReference;
int          __toggle2;


void __toggleLED() {
    if (__toggle2 == HIGH) {
      __toggle2 = LOW;
    } else {
      __toggle2 = HIGH;
    }
    digitalWrite(13, __toggle2);
}


AnalogIF::AnalogIF( uint16_t fundamentalRateHz, uint8_t adcDecimateLog2, uint8_t adcChannel, void *obj, void (*callback)(void *, int) ) {
    // disable interrupts
    cli();

    this->_fundamentalRateHz = fundamentalRateHz;
    this->_adcChannel        = adcChannel;
    this->_adcDecimateLog2   = adcDecimateLog2;
    this->_callback          = callback;
    this->_context           = obj;
    this->_adcAccumulator    = 0;
    this->_adcCounter        = 0;

    __analogIFcbObjReference = this;

    timer_setup(fundamentalRateHz);

    // re-enable iterrupts
    sei();
}

void AnalogIF::startAdc() {
    cli();
    // set up ADC
    ADMUX  = B01000000 | (0xF & _adcChannel);
    ADCSRB = B00000000;

    timer_start();
    sei();
}

void AnalogIF::stopAdc() {
    cli();
    timer_stop();
    sei();
}

void AnalogIF::useAdcChannel( uint8_t adcChannel ) {
    _adcChannel = adcChannel;
}

void AnalogIF::updateAdcSinc( unsigned int adcVal ) {
  if (_adcCounter++ == (unsigned int) (1 << _adcDecimateLog2) ) {
    _callback(this->_context,  (int) _adcAccumulator >> _adcDecimateLog2 );
    _adcCounter = 0;
    _adcAccumulator = 0;
  }
  else
  {
    _adcAccumulator += adcVal;
  }
}

bool AnalogIF::configPwm(PwmConfig config) {

    if (config.pwmMode != SW_PWM) return false;

    _swPwmMinPulseWidth = config.pwmMinPulseWidth;
    _swPwmNumBits      = (config.pwmNumBits > 10) ? 10 : config.pwmNumBits;
    _swPwmPin          = config.pwmPin;

    _swPwmMaxCount     = (1 << _swPwmNumBits);
    _swPwmValueCounter = 0;
    _swPwmClockCounter = 0;
    _swPwmEnable       = false;
    pinMode( _swPwmPin, OUTPUT);

    return true;
}

void AnalogIF::enableSwPwm() {
    _swPwmEnable       = true;
}

void AnalogIF::disableSwPwm() {
  _swPwmEnable       = false;
  _swPwmValueCounter = 0;
  _swPwmClockCounter = 0;
  _swPwmMaxCount     = 0;
  digitalWrite(_swPwmPin, LOW);
}

void AnalogIF::setPwmValue(uint16_t pwmValue ) {
	_pwmValue = pwmValue; // << (10 - _swPwmNumBits);
}

void AnalogIF::updateSwPwm() {

  if (!_swPwmEnable) return;

  _swPwmClockCounter += 1;

  if (_swPwmClockCounter == _swPwmMinPulseWidth) {

    if (_swPwmValueCounter < _pwmValue) {
      digitalWrite( _swPwmPin, HIGH);
    }
    else
    {
      digitalWrite( _swPwmPin, LOW);
    }

    _swPwmValueCounter += 1;
    if (_swPwmValueCounter == _swPwmMaxCount - 1) {
      _swPwmValueCounter = 0;
    }

    _swPwmClockCounter = 0;
  }
}

// TIMER_VECTOR initialized TIMERx_COMPA_vect, where x is the
// selected timer (0, 1, or 2) in TIMER constant in Sampler.h
ISR(TIMER_VECTOR)
{
   ADCSRA = B11001111;
   __analogIFcbObjReference->updateSwPwm();
}

ISR(ADC_vect)
{
    unsigned char adcl = ADCL;
    unsigned char adch = ADCH;
    unsigned int adcVal = (adch << 8) | adcl;

   __analogIFcbObjReference->updateAdcSinc(adcVal);
}
