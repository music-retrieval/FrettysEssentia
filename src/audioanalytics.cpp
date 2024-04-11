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
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

        writer.StartObject();
        writer.Key("key");
        writer.StartObject();
        writer.Key("key");
        writer.String(key.c_str());
        writer.Key("strength");
        writer.Double(keyStrength);
        writer.EndObject();
        
        writer.Key("scale");
        writer.String(scale.c_str());

        writer.Key("chords");
        writer.StartArray();
        for (size_t i = 0; i < chords.size(); ++i) {
            writer.StartObject();
            writer.Key("chord");
            writer.String(chords[i].c_str());
            writer.Key("strength");
            writer.Double(chordStrengths[i]);
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();

        return buffer.GetString();
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

    AudioAnalyticsBuilder& AudioAnalyticsBuilder::withScale(std::string scale) {
        audioAnalytics.scale = scale;

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
