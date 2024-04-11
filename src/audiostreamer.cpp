#include "audiostreamer.h"

namespace audiostreamer {

    AudioStreamer::AudioStreamer() {
        // Initialize Essentia
        essentia::init();
    }

    AudioStreamer::~AudioStreamer() {
        // Cleanup Essentia
        essentia::shutdown();
    }

    Network AudioStreamer::createPipeline(const char* audioFile) {
        // Create an algorithm factory
        AlgorithmFactory& factory = AlgorithmFactory::instance();

        // Loads the audio file
        Algorithm* loader = factory.create("MonoLoader",
                                            "filename", audioFile);

        // Cuts the audio into frames
        Algorithm* framecutter = factory.create("FrameCutter",
                                                "frameSize", 4096,
                                                "hopSize", 2048,
                                                "silentFrames", "noise");

        // Applies a windowing function to the frames
        Algorithm* windowing = factory.create("Windowing",
                                            "type", "blackmanharris62");

        // Computes the spectrum of the frames
        Algorithm* spectrum = factory.create("Spectrum");

        // Computes the spectral peaks
        Algorithm* spectralpeaks = factory.create("SpectralPeaks",
                                                "orderBy", "magnitude",
                                                "magnitudeThreshold", 0.00001,
                                                "minFrequency", 20,
                                                "maxFrequency", 3500,
                                                "maxPeaks", 60);

        // Computes the Harmonic Pitch Class Profile (HPCP)
        Algorithm* hpcp = factory.create("HPCP");

        // Computes the key
        Algorithm* hpcp_key = factory.create("HPCP",
                                            "size", 36,
                                            "referenceFrequency", 440,
                                            "bandPreset", false,
                                            "minFrequency", 20,
                                            "maxFrequency", 3500,
                                            "weightType", "cosine",
                                            "nonLinear", false,
                                            "windowSize", 1.0);

        Algorithm* chordEstimator = factory.create("ChordsDetection");

        // Computes the key
        Algorithm* key = factory.create("Key",
                                        "profileType", "edma",
                                        "numHarmonics", 4,
                                        "pcpSize", 36,
                                        "slope", 0.6,
                                        "usePolyphony", true,
                                        "useThreeChords", true);

        // Connect streaming algorithms together
        loader->output("audio") >> framecutter->input("signal");
        framecutter->output("frame") >> windowing->input("frame");
        windowing->output("frame") >> spectrum->input("frame");
        spectrum->output("spectrum") >> spectralpeaks->input("spectrum");
        spectralpeaks->output("magnitudes") >> hpcp->input("magnitudes");
        spectralpeaks->output("frequencies") >> hpcp->input("frequencies");
        spectralpeaks->output("magnitudes") >> hpcp_key->input("magnitudes");
        spectralpeaks->output("frequencies") >> hpcp_key->input("frequencies");
        hpcp_key->output("hpcp") >> key->input("pcp");
        hpcp->output("hpcp") >> chordEstimator->input("pcp");

        // Store the results in the pool
        hpcp->output("hpcp") >> PC(pool, "tonal.hpcp");
        key->output("key") >> PC(pool, "tonal.key_key");
        key->output("scale") >> PC(pool, "tonal.key_scale");
        key->output("strength") >> PC(pool, "tonal.key_strength");
        chordEstimator->output("chords") >> PC(pool, "chords.value");
        chordEstimator->output("strength") >> PC(pool, "chords.strength");

        // Run the streaming network
        return Network(loader);
    }

    audioanalytics::AudioAnalytics AudioStreamer::processAudio(const char* audioFile) {
        Network network = createPipeline(audioFile);
        network.run();

        string estimatedKey = pool.value<string>("tonal.key_key");
        string estimatedScale = pool.value<string>("tonal.key_scale");

        vector<string> estimatedChords = pool.value<vector<string>>("chords.value");
        vector<float> estimatedChordStrengths = pool.value<vector<float>>("chords.strength");

        cout << "Estimated key: " << estimatedKey << endl;
        cout << "Estimated scale: " << estimatedScale << endl;

        audioanalytics::AudioAnalyticsBuilder builder;
        audioanalytics::AudioAnalytics audioAnalytics = builder.withKey(estimatedKey, pool.value<float>("tonal.key_strength"))
                                                                .withScale(estimatedScale)
                                                                .withChords(estimatedChords, estimatedChordStrengths)
                                                                .build();
        
        return audioAnalytics;
    }
}
