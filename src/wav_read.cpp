#include "wav_read.hpp"
#include <fcntl.h>
#include <cstdint>
#include <climits>
#include <vector>
#include <unistd.h>
#include <string>
#include <stdexcept>


class Wav{
private:
	WAV_HEADER wavHeader; //wave header struct
	// FILE* wavFile;
	// int16_t *wavData;
	std::vector<float> audioData; //will be replaced in the future
public:
	Wav(const char* fileName){
		FILE* wavFile = fopen(fileName,"rb");
		if(wavFile==NULL){
			throw std::runtime_error("Error in reading wav file");
		}
		//size of header
		size_t headerSize=sizeof(WAV_HEADER);
		// reading file header contents into struct WAV_HEADER
		fread(&wavHeader,headerSize,1,wavFile);

		//offset by header size to access data chunk
		fseek(wavFile,headerSize,SEEK_SET);

		//no support for 2 channel sterio so avoiding for now
		if(wavHeader.NumOfChan >1){
			throw std::runtime_error("No support for more than 1 channel audio");
		}

		//allocating data chunk using malloc
		int16_t *wavData=(int16_t*)malloc(wavHeader.Subchunk2Size);
		if(!wavData){
			throw std::runtime_error("Error in allocating chunk data (malloc)");
		}


		size_t bytesRead= fread(wavData,sizeof(int16_t),wavHeader.Subchunk2Size,wavFile);

		if(bytesRead <=0 ){
			throw std::runtime_error("Error in reading chunk data");
		}

		//single channel implementation
		//converting wav data to -1 to 1 range by dividing by max float values and *2 so normalize
		//implementation has to be changed in future
		for(int i=0;i<wavHeader.Subchunk2Size;i++){
			audioData.push_back(2*wavData[i] / static_cast<float>(USHRT_MAX));
		}

		free(wavData);
		fclose(wavFile);
	}

	uint32_t GetChunkSize();
	uint32_t GetSampleRate();
	uint32_t GetNumOfChannels();
	std::vector<float> LoadValues();


};

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

