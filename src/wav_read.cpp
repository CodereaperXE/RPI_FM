#include "wav_read.hpp"
#include <fcntl.h>
#include <cstdint>
#include <climits>
#include <vector>
#include <unistd.h>
#include <string>
#include <stdexcept>


//get data chunk size
uint32_t Wav::GetChunkSize(){
	return wavHeader.Subchunk2Size;
}
//get sample rate
uint32_t Wav::GetSampleRate(){
	return wavHeader.SamplesPerSec;
}
//get number of channels
uint32_t Wav::GetNumOfChannels(){
	return wavHeader.NumOfChan;
}
//get loaded values
std::vector<float> Wav::LoadValues(){
	return audioData;
}

