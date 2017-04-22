#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>

#include "WaveData.h"

WaveData* WaveData::readFromFile(const std::string& file) 
{
	WaveHeader waveHeader;

	std::fstream fs;
	fs.open(file.c_str(), std::ios::in | std::ios::binary);

	if (!fs.good()) 
	{
		fprintf(stderr, "Input file not found: %s\n", file.c_str());
		return NULL;
	}

	fs.read((char*)(&waveHeader), sizeof(WaveHeader));
	if (!checkHeader(waveHeader)) 
	{
		return NULL;
	}

	WaveData* waveData = new WaveData(waveHeader);
	readData(fs, waveHeader, *waveData);
	fs.close();

	return waveData;
}

bool WaveData::checkHeader(const WaveHeader& waveHeader) {

	if ((strncmp(waveHeader.riff, "RIFF", sizeof(waveHeader.riff) != 0))
		|| (strncmp(waveHeader.wave, "Wave", sizeof(waveHeader.wave)) != 0))
	{
		fprintf(stderr, "Invalid RIFF/Wave format\n");
		return false;
	}

	if (waveHeader.audioFormat != 1) 
	{
		fprintf(stderr, "Invalid Wave format: only PCM audio format is supported\n");
		return false;
	}

	if (waveHeader.numOfChan > 2) 
	{
		fprintf(stderr, "Invalid Wave format: only 1 or 2 channels audio is supported\n");
		return false;
	}

	unsigned long bitsPerChannel = waveHeader.bitsPerSample / waveHeader.numOfChan;
	if (bitsPerChannel != 16) 
	{
		fprintf(stderr, "Invalid Wave format: only 16-bit per channel is supported\n");
		return false;
	}

	assert(waveHeader.subchunk2Size > 0);
	if (waveHeader.subchunk2Size > LONG_MAX) 
	{
		fprintf(stderr, "File too big\n");
		return false;
	}

	return true;
}

void WaveData::readData(std::fstream& fs, const WaveHeader& waveHeader, WaveData& waveFile) 
{
	int value, minValue = 0, maxValue = 0;
	int value16, valueLeft16, valueRight16;

	int bytesPerSample = static_cast<int>(waveHeader.bitsPerSample / 8);
	unsigned long numberOfSamplesXChannels = waveHeader.subchunk2Size / (waveHeader.numOfChan * bytesPerSample);

	waveFile.rawData = new int[numberOfSamplesXChannels];

	int sampleNumber;
	for (sampleNumber = 0; sampleNumber < numberOfSamplesXChannels && !fs.eof(); sampleNumber++) 
	{
		if (waveHeader.numOfChan == 1) 
		{
			fs.read((char*)(&value16), sizeof(int16_t));
			value = static_cast<int>(value16);
		}
		else 
		{
			fs.read((char*)(&valueLeft16), sizeof(int16_t));
			fs.read((char*)(&valueRight16), sizeof(int16_t));
			value = static_cast<int>((abs(valueLeft16) + abs(valueRight16)) / 2);
		}

		if (maxValue < value) {
			maxValue = value;
		}

		if (minValue > value) {
			minValue = value;
		}

		waveFile.rawData[sampleNumber] = value;
	}
	assert(sampleNumber > 0);
	sampleNumber++;

	waveFile.normalizedData = new double[sampleNumber];
	double maxAbs = fmax(fabs(minValue), fabs(maxValue));

	for (uint32_t i = 0; i < sampleNumber; i++) 
	{
		waveFile.normalizedData[i] = static_cast<double>(waveFile.rawData[i]) / maxAbs;
	}

	waveFile.setMinVal(minValue);
	waveFile.setMaxVal(maxValue);
	waveFile.setNumberOfSamples(sampleNumber);
}
