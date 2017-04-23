#include "Frame.h"

Frame::Frame(uint32_t id) : id(id), mfcc(0) 
{ }

std::vector<double> Frame::initMFCC(const std::vector<double> dataFrame, uint32_t frequency) 
{
	this->mfcc = MFCC::computeMFCC(dataFrame, frequency);
	return this->mfcc;
}
