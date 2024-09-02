
#include <fcntl.h>
#include <cstdint>
#include <climits>
#include <vector>
#include <unistd.h>
#include <string>
#include <stdexcept>

//wav file header
typedef struct 
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
} WAV_HEADER;

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
        int16_t *wavData=(int16_t*)malloc(wavHeader.Subchunk2Size*sizeof(int16_t));
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




