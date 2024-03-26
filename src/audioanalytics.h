#ifndef AUDIOANALYTICS_H
#define AUDIOANALYTICS_H

#include <string>
#include <sstream>
#include <vector>

namespace audioanalytics {

    class AudioAnalytics {
        public:
            std::string key;
            std::string scale;
            float keyStrength;
            std::vector<std::string> chords;
            std::vector<float> chordStrengths;

            const std::string forNetwork();
    };

    class AudioAnalyticsBuilder {
        private:
            AudioAnalytics audioAnalytics;

        public:
            AudioAnalyticsBuilder();

            AudioAnalyticsBuilder& withKey(std::string key, float keyStrength);

            AudioAnalyticsBuilder& withChords(std::vector<std::string> chords, std::vector<float> chordStrengths);

            AudioAnalytics build();
    };
}

#endif /* AUDIOANALYTICS_H */
