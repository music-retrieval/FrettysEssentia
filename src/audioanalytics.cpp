#include "audioanalytics.h"

namespace audioanalytics {
    // Function to concatenate vector of strings into a single string with a delimiter
    std::string concatenateStrings(const std::vector<std::string>& strings, const std::string& delimiter) {
        std::ostringstream oss;
        for (size_t i = 0; i < strings.size(); ++i) {
            if (i != 0) {
                oss << delimiter;
            }
            oss << strings[i];
        }
        return oss.str();
    }

    // Prepares audio analytics data for network transmission
    const std::string AudioAnalytics::forNetwork() {
        return concatenateStrings(chords, ",");
    }

    // Builder pattern for audio analytic daa
    AudioAnalyticsBuilder::AudioAnalyticsBuilder() {
        audioAnalytics = AudioAnalytics();
    }

    AudioAnalyticsBuilder& AudioAnalyticsBuilder::withKey(std::string key, float keyStrength) {
        audioAnalytics.key = key;
        audioAnalytics.keyStrength = keyStrength;

        return *this;
    }

    AudioAnalyticsBuilder& AudioAnalyticsBuilder::withChords(std::vector<std::string> chords, std::vector<float> chordStrengths) {
        audioAnalytics.chords = chords;
        audioAnalytics.chordStrengths = chordStrengths;
        
        return *this;
    }

    AudioAnalytics AudioAnalyticsBuilder::build() {
        return audioAnalytics;
    }
}
