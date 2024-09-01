#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sndfile.h>        // For reading WAV files

// Function to compute the FM signal
double computeFMSignal(double t, double sampleRate, double carrierFreq, double peakDeviation,
                       const std::vector<double>& modulatingSignal, size_t modulatingSignalOffset, size_t numSamples,
                       double amplitude) {
    double phaseDeviation = 0.0;
    double dt = 1.0 / sampleRate;  // Time step

    // Integrate the modulating signal to calculate phase deviation
    for (size_t i = 0; i < numSamples; ++i) {
        phaseDeviation += modulatingSignal[modulatingSignalOffset + i] * dt;
    }
    phaseDeviation *= 2 * M_PI * peakDeviation;

    // Compute the FM signal
    double phase = 2 * M_PI * carrierFreq * t + phaseDeviation;
    double fmSignal = amplitude * std::cos(phase);

    return fmSignal;
}

int main() {
    // Open the WAV file
    const char *filename = "acoustic_guitar_duet.wav";
    SF_INFO sfinfo;
    SNDFILE *sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    // Read the WAV file data
    std::vector<double> samples(sfinfo.frames);
    sf_readf_double(sndfile, samples.data(), sfinfo.frames);
    sf_close(sndfile);

    // Parameters for FM signal
    double sampleRate = sfinfo.samplerate;
    double carrierFreq = 1000;  // Carrier frequency in Hz
    double peakDeviation = 500; // Peak frequency deviation in Hz
    double amplitude = 1.0;     // Amplitude of the carrier signal

    // Compute the FM signal
    std::ofstream outputFile("fm_signal.csv");
    if (!outputFile) {
        std::cerr << "Error: Could not open output file" << std::endl;
        return 1;
    }
    outputFile << "Time,FM_Signal\n";
    for (size_t i = 0; i < samples.size(); ++i) {
        double t = static_cast<double>(i) / sampleRate;
        double fmSignal = computeFMSignal(t, sampleRate, carrierFreq, peakDeviation, samples, 0, samples.size(), amplitude);
        outputFile << t << "," << fmSignal << "\n";
    }
    outputFile.close();

    std::cout << "FM signal data written to fm_signal.csv" << std::endl;

    return 0;
}
