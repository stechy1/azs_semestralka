/*
 * SignalGenerator.h
 *
 *  Created on: 20. 2. 2018
 *      Author: petr
 */

#ifndef SIGNALGENERATOR_H_
#define SIGNALGENERATOR_H_

#define PI 3.14159265358979323846

class SignalGenerator {
public:
	SignalGenerator(int frequency, int amplitude);
	virtual ~SignalGenerator();

	enum SignalType {
			Sine,
			Square,
			Triangle,
			Sawtooth,
			Off
		};

	void setSignalType(SignalType newType);
	int getValue(int time);


private:
	SignalType signalType;
	int frequency;
	int amplitude;
	int offset;
};

#endif /* SIGNALGENERATOR_H_ */
