#ifndef WAVEHEADER_H_
#define WAVEHEADER_H_

/**
* WaveHeader for PCM format
* information http://audiocoding.ru/article/2008/05/22/wav-file-structure.html
*/
struct WaveHeader 
{
	
	char chunkId[4]; // "riff"
	unsigned long chunkSize; // size - 8 (chunkId and chunkSize)
	char format[4]; // "wave"

	// format "WAVE" consist from 2 subchunk: "fmt " and "data":
	// subchunk "fmt" describes format of audio data

	char subchunk1Id[4]; // "fmt "
	unsigned long subchunk1Size; // size of the remaining subchunk; for PCM = 16
	unsigned short audioFormat; // for PCM = 1, otherwise some compression format
	unsigned short numChannels; // number of channels 1=Mono, 2=Stereo
	unsigned long sampleRate; // sampling frequency in Hz
	unsigned long byteRate; // bytes per second (sampleRate * numChannels * bitsPerSample/8)
	unsigned short blockAlign; // number of bytes for sample, including all channels (numChannels * bitsPerSample/8)
	unsigned short bitsPerSample;// number of bits per sample

	// subchunk "data" contains audio data and its size.

	char subchunk2Id[4];//"data"
	unsigned long subchunk2Size; // sampled data length
};
#endif /* WAVEHEADER_H_ */