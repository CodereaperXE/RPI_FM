#include <iostream>
#include <fcntl.h>
#include <stdint.h>
#include <climits>
#include <vector>
#include "read_wav.hpp"

typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;




 std::vector<float> get_values(){
 	std::vector<float> a;
	//wav file header reading;
	FILE* wavfile;

	wav_hdr wav_header;

	wavfile=fopen("acoustic_guitar_duet.wav","rb");

	size_t header_size=sizeof(wav_hdr);

	// std::cout<<header_size;

	fread(&wav_header,header_size,1,wavfile);
	
	// std::cout<<wav_header.Subchunk2Size<<std::endl;
	// fclose(wavfile);

	//wav file data reading
	fseek(wavfile,header_size,SEEK_SET);

	// std::cout<<wav_header.Subchunk2Size;
	// std::cout<<datachunk_size;	
	int16_t *data=(int16_t*)malloc(wav_header.Subchunk2Size);
	if(!data){
		std::cerr<<"error";
	}

	std::cout<<fread(data,sizeof(int16_t),wav_header.Subchunk2Size,wavfile) <<std::endl;

	std::cout<<wav_header.NumOfChan<<std::endl;
	std::cout<<wav_header.Subchunk2Size<<std::endl;
	for(int i=0;i<wav_header.Subchunk2Size;i++){
		a.push_back(2*data[i]/static_cast<float>(USHRT_MAX));
		// if(i%2==0) {std::cout<<std::endl;}
	}

	//writing data for testing;

	free(data);
	fclose(wavfile);
	return a;




}