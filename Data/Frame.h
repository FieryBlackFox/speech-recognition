#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>
#include <vector>
#include "../MFCC/MFCC.h"

/**
* Frame - short fragment of a signal
*/
class Frame 
{
	const uint32_t id;

	double rms; //root mean square
	double entropy;

	std::vector<double> mfcc; //mel-frecuency cepstral coefficient

public:
	/**
	* Create a frame
	*/
	Frame(uint32_t id);

	/**
	* Get frame's serial number
	*/
	uint32_t getId() const 
	{ 
		return this->id; 
	}

	/**
	* Get Mel-frequency cepstral coefficients
	*/
	std::vector<double> getMFCC() const 
	{ 
		return mfcc; 
	}

	/**
	* Get Mel-frequency cepstral coefficients
	*/
	std::vector<double> initMFCC(const std::vector<double> dataFrame, uint32_t frequency);
};

#endif /* FRAME_H_ */