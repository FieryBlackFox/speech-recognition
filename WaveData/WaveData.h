#ifndef WAVEDATA_H_
#define WAVEDATA_H_

#include <cstdint>
#include <iostream>
#include <string>

#include"WaveHeader.h"

class WaveData 
{
	WaveHeader header;
	int* rawData;
	double* normalizedData;

	int maxVal;
	int minVal;
	int numberOfSamples;

	WaveData(WaveHeader header) {
		this->header = header;
		this->rawData = NULL;
		this->normalizedData = NULL;

		this->maxVal = 0;
		this->minVal = 0;
		this->numberOfSamples = 0;
	}

	static bool checkHeader(const WaveHeader& waveHeader);
	static void readData(std::fstream& fs, const WaveHeader& waveHeader, WaveData& waveFile);

public:
	~WaveData() {
		if (this->rawData != NULL) {
			delete[] this->rawData;
		}
		if (this->normalizedData != NULL) {
			delete[] this->normalizedData;
		}
	}

	static WaveData* readFromFile(const std::string& file);

	int getNumberOfSamples() const 
	{ 
		return numberOfSamples; 
	}
	void setNumberOfSamples(int numberOfSamples) 
	{ 
		this->numberOfSamples = numberOfSamples; 
	}

	int getMaxVal() const 
	{ 
		return maxVal; 
	}
	void setMaxVal(int maxVal) 
	{ 
		this->maxVal = maxVal; 
	}

	int getMinVal() const 
	{ 
		return minVal; 
	}
	void setMinVal(int minVal) 
	{ 
		this->minVal = minVal; 
	}

	const WaveHeader& getHeader() const 
	{ 
		return header; 
	}
	const int* getRawData() const 
	{ 
		return rawData; 
	}
	const double* getNormalizedData() const 
	{
		return normalizedData; 
	}
};


#endif /* WAVEDATA_H_ */
