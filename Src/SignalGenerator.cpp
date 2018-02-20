/*
 * SignalGenerator.cpp
 *
 *  Created on: 20. 2. 2018
 *      Author: petr
 */

#include "SignalGenerator.h"
#include "math.h"

SignalGenerator::SignalGenerator(int frequency, int amplitude) {
	this->frequency = frequency;
	this->offset = 0;
	this->amplitude = amplitude;
	this->signalType = Off;
}

SignalGenerator::~SignalGenerator() {
}

void SignalGenerator::setSignalType(SignalType newType) {
	this->signalType = newType;
}

int SignalGenerator::getValue(int time) {
	float value = 0f;
	float t = frequency * time;
	switch (signalType) { // http://en.wikipedia.org/wiki/Waveform
		case SignalType.Sine: // sin( 2 * pi * t )
			//value = (float)Math.Sin(2f*Math.PI*t);
			value = sin((float)(2f*PI*t));
			break;
		case SignalType.Square: // sign( sin( 2 * pi * t ) )
			//value = Math.Sign(Math.Sin(2f*Math.PI*t));
			value = signbit(sin(2f*PI*t));
			break;
		case SignalType.Triangle:
		// 2 * abs( t - 2 * floor( t / 2 ) - 1 ) - 1
			//value = 1f-4f*(float)Math.Abs(Math.Round(t-0.25f)-(t-0.25f));
			value = 1f-4f*(float)abs(round(t-0.25f)-(t-0.25f));
			break;
		case SignalType.Saw:
		// 2 * ( t/a - floor( t/a + 1/2 ) )
			//value = 2f*(t-(float)Math.Floor(t+0.5f));
			value = 2f*(t-(float)floor(t+0.5f));
			break;
	}

	return(amplitude*value+offset);

	//return 0;
}
