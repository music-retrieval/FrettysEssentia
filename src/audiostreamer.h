#ifndef AUDIOSTREAMER_H
#define AUDIOSTREAMER_H

#include <iostream>
#include <string>
#include <essentia/algorithmfactory.h>
#include <essentia/scheduler/network.h>
#include <essentia/streaming/algorithms/poolstorage.h>

#include "audioanalytics.h"

using namespace std;
using namespace essentia;
using namespace essentia::streaming;
using namespace essentia::scheduler;

namespace audiostreamer {

    class AudioStreamer {
        public:
            AudioStreamer();
            ~AudioStreamer();

            audioanalytics::AudioAnalytics processAudio(const char* audioFile);

        private:
            Pool pool;

            Network createPipeline(const char* audioFile);
    };
}

#endif /* AUDIOSTREAMER_H */
