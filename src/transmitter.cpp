#include "transmitter.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <climits>
#include <thread>
#include <vector>


// class Transmitter:public PeripheralSetup{
	Transmitter::Transmitter(){}
	Transmitter::~Transmitter(){
		if(pTransmitViaCpu){
			//stop thread;
			threadStopSignal=1;
			//wait for thread to finish
			pTransmitViaCpu->join();
		}
	}
// private:
	// float frequency=100.f; //base frequency
	// float bandwidth=200.f; //base bandwidth
	// bool threadStopSignal=0; //thread flag
	// std::unique_ptr<std::thread> pTransmitViaCpu; //thread pointer to TransmitViaCpu thread
	// std::vector<float> audioData;

	// void TransmitViaCpu(float frequency,std::vector<float>& audioData,uint32_t sampleRate,float bandwidth);
// public:
	// void SetParams(float bandwidth);
	// void StartTransmit(float frequency,std::vector<float>& audioData,uint32_t sampleRate,float bandwidth);
	// void StopTransmit();
// };

// void Transmitter::SetParams(float frequency,std::vector<float> audioData,float bandwidth=200.f){
// 	this->audioData = std::move(audioData);
// 	this->frequency = frequency;
// 	this->bandwidth = bandwidth;
// }

void Transmitter::TransmitViaCpu(float frequency,std::vector<float>& audioData,uint32_t sampleRate,float bandwidth){
	//setup
	//disable clock
	*clk = CLK_MANAGER_PASSWD | (1<<5); //disable clock
	std::this_thread::sleep_for(std::chrono::microseconds(100));

	//set PLLD as source
	*clk = CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLD;

	//calculate divisor
	uint32_t divisor=static_cast<uint32_t>(round((PLLD_FREQUENCY * (1<<12)) / frequency));

	//calculate divisor range
	unsigned divisorRange = divisor - static_cast<uint32_t>(round((PLLD_FREQUENCY * (1<<12)) / (frequency + 0.0005f * bandwidth)));
	
	//configure clock
	*div = CLK_MANAGER_PASSWD | divisor;

	//enable clock
	*clk = CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLD | GPCLK_CTL_ENAB | GPCLK_CTL_MASH(0x1);

	//transmit
	unsigned offset = 0;

	// used to measure time from start to current 
	std::chrono::system_clock::time_point current_t, start_t;
	start_t = current_t = std::chrono::system_clock::now(); //initial time sample

	while(true){
		//if thread is asked to stop
		if(threadStopSignal){
			break;
		}

		//if offset crosses size of audioData vector break
		if(offset >= audioData.size()){
			break;
		}

		//previous offset
		unsigned prevOffset = offset;

		float value = audioData[offset]; //fetching data by offset from audiodata

		//setting divisor to cause fm modulation
		*div = CLK_MANAGER_PASSWD | (divisor - static_cast<int>(round(value * divisorRange))); //adjusting value to scale with divisorRange limits
		
		while(offset == prevOffset){ //finding the new offset and making sure offset is different else repeat until new offset is achieved
			std::this_thread::yield(); //asm("nop") equivalent (ie waiting for some time)
			current_t = std::chrono::system_clock::now();
			//calculating offset by multiplying time spent by sample rate to determine the offset from start of the vector (divided by 1000000 to convert from microseconds to seconds)
			offset = std::chrono::duration_cast<std::chrono::microseconds>(current_t - start_t).count() * sampleRate /1000000;

		}

	}

}


void Transmitter::StartTransmit(float frequency,std::vector<float>& audioData,uint32_t sampleRate,float bandwidth){
	//create a thread pointer to transmitviacpu method
	pTransmitViaCpu = std::make_unique<std::thread>(&Transmitter::TransmitViaCpu,this,frequency,std::ref(audioData),sampleRate,bandwidth);
	pTransmitViaCpu->join(); //wait for thread to finish
}

void Transmitter::StopTransmit(){
	if(pTransmitViaCpu){
			//stop thread;
			threadStopSignal=1;
			//wait for thread to finish
			pTransmitViaCpu->join();
	}
}