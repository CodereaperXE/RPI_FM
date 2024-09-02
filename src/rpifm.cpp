#include "wav_read.hpp"
#include "transmitter.hpp"
#include <vector>

int main(){
	const char* file="acoustic_guitar_duet.wav";
	Wav re(file);
	std::vector<float> a;
	a=re.LoadValues();

	Transmitter t;
	float frequency=100.f;
	float bandwidth=200.f;

	t.StartTransmit(frequency,a,re.GetSampleRate(),bandwidth);

}