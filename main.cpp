#include"../Data/WaveData.h"
#include<cassert>
#include"../Data/Frame.h"
#include<map>
#include<iostream>
#include<chrono>


int main() 
{
	auto start = std::chrono::high_resolution_clock::now();
	WaveData* waveData = WaveData::readFromFile("..\\male1.wav");
	std::vector<Frame*>* frames = new std::vector<Frame*>();
	std::map<uint32_t, std::pair<uint32_t, uint32_t> >* frameToRaw = new std::map<uint32_t, std::pair<uint32_t, uint32_t> >();

	uint32_t bytesPerFrame = (uint32_t)(waveData->getHeader().byteRate * FRAME_LENGTH / 1000.0);
	uint32_t bytesPerSample = (uint32_t)(waveData->getHeader().bitsPerSample / 8);
	uint32_t samplesPerFrame = (uint32_t)(bytesPerFrame / bytesPerSample);
	assert(samplesPerFrame > 0);

	unsigned int samplesPerNonOverlap = (unsigned int)(samplesPerFrame * (1 - FRAME_OVERLAP));
	unsigned int framesCount = (waveData->getHeader().subchunk2Size / (waveData->getHeader().bitsPerSample / 8))/ samplesPerNonOverlap;

	uint32_t indexBegin = 0, indexEnd = 0;
	for (uint32_t frameId = 0, size = waveData->getNumberOfSamples(); frameId < framesCount; ++frameId) 
	{
		indexBegin = frameId * samplesPerNonOverlap;
		indexEnd = indexBegin + samplesPerFrame;
		if (indexEnd < size) {

			Frame* frame = new Frame(frameId);

			frames->insert(frames->begin() + frameId, frame);
			frameToRaw->insert(std::make_pair(frameId, std::make_pair(indexBegin, indexEnd)));
		}
		else {
			break;
		}
	}

	std::vector<std::vector<double> > mfcc;
	std::vector<double> normalizeData = waveData->getNormalizedData();
	for (int i = 0; i < frames->size(); i++)
	{
		int begin = (*frameToRaw)[i].first;
		int end = (*frameToRaw)[i].second;

		std::vector<double> part(normalizeData.begin() + begin, normalizeData.begin() + end);
		mfcc.push_back(frames->at(i)->initMFCC(part, waveData->getHeader().sampleRate));
		
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
	std::cin.get();
	std::cin.get();
	return 0;
}