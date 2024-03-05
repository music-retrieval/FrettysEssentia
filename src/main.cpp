#include <iostream>
#include <vector>
#include <essentia/algorithmfactory.h>

extern "C" {

    // Test fuction for interop
    void test() {
        std::cout << "Hello from C++!" << std::endl;
    }

    // Initiate the essentia library
    void essentia_init() {
        essentia::init();
    }

    // Shutdown the essentia library
    void essentia_shutdown() {
        essentia::shutdown();
    }

    // Load audio from file - actual implementation needs investigation.
    void essentia_load_audio(const char* audioFilename, double* audioBuffer, size_t bufferSize) {
        std::vector<essentia::Real> vecAudioBuffer;
        essentia::standard::Algorithm* audioLoader = essentia::standard::AlgorithmFactory::create("MonoLoader",
                                                                                                    "filename", audioFilename);
        audioLoader->output("audio").set(vecAudioBuffer);
        audioLoader->compute();

        // Ensure the buffer size matches
        size_t minSize = std::min(vecAudioBuffer.size(), bufferSize);

        // Copy data to the provided buffer
        for (size_t i = 0; i < minSize; ++i) {
            audioBuffer[i] = vecAudioBuffer[i];
        }
    }
}
