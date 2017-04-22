#ifndef WAVEHEADER_H_
#define WAVEHEADER_H_

struct WaveHeader {
	char riff[4];
	unsigned long chunkSize;
	char wave[4];
	char fmt[4];
	unsigned long subchunk1Size;
	unsigned short audioFormat;
	unsigned short numOfChan;
	unsigned long samplesPerSec;
	unsigned long bytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char data[4];
	unsigned long subchunk2Size;
};
#endif /* WAVEHEADER_H_ */
