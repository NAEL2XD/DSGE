#include "sound.hpp"
#include "dsge.hpp"
#include <tremor/ivorbisfile.h>
#include <cstring>

namespace {
    struct AudioChannel {
        bool active = false;
        int channel_id;
        ndspWaveBuf waveBufs[3];
        int16_t* audioBuffer = nullptr;
        OggVorbis_File vorbisFile;
        Thread threadId = nullptr;
        bool quit = false;
        bool loop = false;
        int* timePtr = nullptr; // Pointer to track time
    };

    AudioChannel channels[24];
    LightEvent s_event;
    bool system_initialized = false;

    void audioThread(void* arg);
    bool fillBuffer(AudioChannel* channel);
    bool audioInit(AudioChannel* channel);
    void audioExit(AudioChannel* channel);
    void stopChannel(AudioChannel* channel);
}

namespace dsge {

Sound::Sound(const std::string& path) : 
    length(0), time(0), volume(1.0f), loop(false),
    filePath("romfs:/" + path) {
    
    if (!system_initialized) {
        LightEvent_Init(&s_event, RESET_ONESHOT);
        ndspSetCallback([](void* nul) { LightEvent_Signal(&s_event); }, nullptr);
        system_initialized = true;
    }
    
    calculateLength();
}

Sound::~Sound() {
    stop();
}

void Sound::calculateLength() {
    FILE* fh = fopen(filePath.c_str(), "rb");
    if (fh) {
        OggVorbis_File vf;
        if (ov_open(fh, &vf, nullptr, 0) == 0) {
            length = static_cast<int>(ov_time_total(&vf, -1));
            ov_clear(&vf);
        } else {
            fclose(fh);
        }
    }
}

void Sound::play() {
    if (isPlaying) {
        if (isPaused) resume();
        return;
    }

    for (int i = 0; i < 24; i++) {
        if (!channels[i].active) {
            channel = i;
            break;
        }
    }
    
    if (channel == -1) return;
    
    AudioChannel* ch = &channels[channel];
    ch->loop = loop;
    ch->timePtr = &time; // Set time pointer
    
    FILE* fh = fopen(filePath.c_str(), "rb");
    if (!fh) return;
    
    if (ov_open(fh, &ch->vorbisFile, nullptr, 0) != 0) {
        fclose(fh);
        return;
    }
    
    if (!audioInit(ch)) {
        ov_clear(&ch->vorbisFile);
        return;
    }
    
    ndspChnSetMix(channel, (float[12]){volume, volume, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    
    ch->quit = false;
    int32_t priority = 0x30;
    svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
    priority = (priority - 1 < 0x18) ? 0x18 : priority - 1;
    
    ch->threadId = threadCreate(audioThread, ch, 32 * 1024, priority, -1, false);
    if (!ch->threadId) {
        audioExit(ch);
        ov_clear(&ch->vorbisFile);
        return;
    }
    
    ch->active = true;
    isPlaying = true;
    isPaused = false;
    time = 0;
}

void Sound::pause() {
    if (!isPlaying || isPaused || channel == -1) return;
    ndspChnSetPaused(channel, true);
    isPaused = true;
}

void Sound::resume() {
    if (!isPlaying || !isPaused || channel == -1) return;
    ndspChnSetPaused(channel, false);
    isPaused = false;
}

void Sound::stop() {
    if (!isPlaying || channel == -1) return;
    stopChannel(&channels[channel]);
    channel = -1;
    isPlaying = false;
    isPaused = false;
    time = 0;
}

bool Sound::replay() {
    if (channel == -1) {
        play();
        return isPlaying;
    }
    
    AudioChannel* ch = &channels[channel];
    
    // Pause playback
    ndspChnSetPaused(channel, true);
    
    // Rewind to beginning
    if (ov_time_seek(&ch->vorbisFile, 0) != 0) {
        ndspChnSetPaused(channel, false);
        return false;
    }
    
    // Free old buffer
    if (ch->audioBuffer) {
        linearFree(ch->audioBuffer);
        ch->audioBuffer = nullptr;
    }
    
    // Reset channel
    ndspChnReset(channel);
    
    // Reinitialize
    if (!audioInit(ch)) {
        ndspChnSetPaused(channel, false);
        return false;
    }
    
    // Update volume in case it changed
    ndspChnSetMix(channel, (float[12]){volume, volume, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    
    // Resume playback
    ndspChnSetPaused(channel, false);
    isPaused = false;
    time = 0;
    
    return true;
}

} // namespace dsge

// Internal implementation
namespace {

bool fillBuffer(AudioChannel* channel) {
    for (size_t i = 0; i < 3; ++i) {
        if (channel->waveBufs[i].status != NDSP_WBUF_DONE) continue;
        
        int totalBytes = 0;
        while (static_cast<size_t>(totalBytes) < channel->waveBufs[i].nsamples * sizeof(s16)) {
            int16_t* buffer = channel->waveBufs[i].data_pcm16 + (totalBytes / sizeof(s16));
            const size_t bufferSize = (channel->waveBufs[i].nsamples * sizeof(s16) - totalBytes);

            const int bytesRead = ov_read(&channel->vorbisFile, (char*)buffer, bufferSize, nullptr);
            if (bytesRead <= 0) {
                if (bytesRead == 0) {
                    if (channel->loop) {
                        ov_time_seek(&channel->vorbisFile, 0);
                        *channel->timePtr = 0;
                        continue;
                    }
                    break;
                }
                break;
            }
            totalBytes += bytesRead;
            
            // Update time position
            *channel->timePtr = static_cast<int>(ov_time_tell(&channel->vorbisFile));
        }

        if (totalBytes == 0) return false;

        channel->waveBufs[i].nsamples = totalBytes / sizeof(s16);
        ndspChnWaveBufAdd(channel->channel_id, &channel->waveBufs[i]);
        DSP_FlushDataCache(channel->waveBufs[i].data_pcm16, totalBytes);
    }
    return true;
}

void audioThread(void* arg) {
    AudioChannel* channel = (AudioChannel*)arg;
    
    while (!channel->quit) {
        if (!fillBuffer(channel)) {
            if (!channel->loop) break;
        }
        LightEvent_Wait(&s_event);
    }
    
    channel->active = false;
    *channel->timePtr = 0;
}

bool audioInit(AudioChannel* channel) {
    vorbis_info* vi = ov_info(&channel->vorbisFile, -1);
    
    ndspChnReset(channel->channel_id);
    ndspChnSetInterp(channel->channel_id, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(channel->channel_id, vi->rate);
    ndspChnSetFormat(channel->channel_id, vi->channels == 1 ? NDSP_FORMAT_MONO_PCM16 : NDSP_FORMAT_STEREO_PCM16);

    const size_t SAMPLES_PER_BUF = vi->rate * 120 / 1000;
    const size_t CHANNELS_PER_SAMPLE = vi->channels;
    const size_t WAVEBUF_SIZE = SAMPLES_PER_BUF * CHANNELS_PER_SAMPLE * sizeof(s16);
    const size_t bufferSize = WAVEBUF_SIZE * 3;
    
    channel->audioBuffer = (int16_t*)linearAlloc(bufferSize);
    if (!channel->audioBuffer) return false;

    memset(&channel->waveBufs, 0, sizeof(channel->waveBufs));
    int16_t* buffer = channel->audioBuffer;

    for (size_t i = 0; i < 3; ++i) {
        channel->waveBufs[i].data_vaddr = buffer;
        channel->waveBufs[i].nsamples = WAVEBUF_SIZE / sizeof(buffer[0]);
        channel->waveBufs[i].status = NDSP_WBUF_DONE;
        buffer += WAVEBUF_SIZE / sizeof(buffer[0]);
    }

    return true;
}

void stopChannel(AudioChannel* channel) {
    if (!channel->active) return;
    
    if (channel->threadId) {
        channel->quit = true;
        LightEvent_Signal(&s_event);
        threadJoin(channel->threadId, UINT64_MAX);
        threadFree(channel->threadId);
        channel->threadId = nullptr;
    }

    if (channel->vorbisFile.datasource) {
        ov_clear(&channel->vorbisFile);
    }
    
    audioExit(channel);
    channel->active = false;
}

void audioExit(AudioChannel* channel) {
    ndspChnReset(channel->channel_id);
    if (channel->audioBuffer) {
        linearFree(channel->audioBuffer);
        channel->audioBuffer = nullptr;
    }
}

} // namespace