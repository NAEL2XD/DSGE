#include "dsge.hpp"
#include "sound.hpp"
#include <tremor/ivorbisfile.h>
#include <cstring>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

namespace {
    struct AudioChannel {
        bool active = false;
        int channel_id;
        ndspWaveBuf waveBufs[3];
        int16_t* audioBuffer = nullptr;
        OggVorbis_File vorbisFile;
        Thread threadId = nullptr;
        bool quit = false;
    };

    AudioChannel channels[24];
    LightEvent s_event;
    bool system_initialized = false;

    // Audio thread function
    void audioThread(void* arg);
    
    // Helper functions
    bool fillBuffer(AudioChannel* channel);
    bool audioInit(AudioChannel* channel);
    void audioExit(AudioChannel* channel);
    const char* vorbisStrError(int error);
    void stopChannel(AudioChannel* channel);

    // NDSP audio callback
    void audioCallback(void* nul) {
        (void)nul;
        LightEvent_Signal(&s_event);
    }
}

namespace dsge {
namespace Sound {

void init() {
    if (system_initialized) return;
    
    LightEvent_Init(&s_event, RESET_ONESHOT);
    ndspSetCallback(audioCallback, nullptr);
    
    for (int i = 0; i < 24; i++) {
        channels[i].channel_id = i;
    }
    
    system_initialized = true;
}

int play(const std::string& path) {
    if (!system_initialized) return -1;

    // Find first available channel
    AudioChannel* channel = nullptr;
    for (int i = 0; i < 24; i++) {
        if (!channels[i].active) {
            channel = &channels[i];
            break;
        }
    }
    
    if (!channel) {
        print("Sound::play: No available audio channels\n");
        return -1;
    }
    
    // Open Ogg Vorbis file
    FILE* fh = fopen(("romfs:/" + path).c_str(), "rb");
    if (!fh) {
        print("Sound::play: Failed to open file: " + path);
        return -1;
    }
    
    memset(&channel->vorbisFile, 0, sizeof(OggVorbis_File));
    int error = ov_open(fh, &channel->vorbisFile, nullptr, 0);
    if (error) {
        print("ov_open: error " + std::to_string(error) + " (" + vorbisStrError(error) + ")");
        fclose(fh);
        return -1;
    }
    
    // Initialize audio system for this channel
    if (!audioInit(channel)) {
        print("Sound::play: Failed to initialize audio channel " + channel->channel_id);
        ov_clear(&channel->vorbisFile);
        return -1;
    }
    
    // Create audio thread
    int32_t priority = 0x30;
    svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
    priority = (priority - 1 < 0x18) ? 0x18 : priority - 1;
    priority = (priority > 0x3F) ? 0x3F : priority;
    
    channel->quit = false;
    channel->threadId = threadCreate(audioThread, channel, 32 * 1024, priority, -1, false);
    if (!channel->threadId) {
        print("Sound::play: Failed to create audio thread");
        audioExit(channel);
        ov_clear(&channel->vorbisFile);
        return -1;
    }
    
    channel->active = true;
    return channel->channel_id;
}

void pause(int channel) {
    if (channel < 0 || channel >= 24 || !channels[channel].active) return;
    ndspChnSetPaused(channel, true);
}

void resume(int channel) {
    if (channel < 0 || channel >= 24 || !channels[channel].active) return;
    ndspChnSetPaused(channel, false);
}

void stop(int channel) {
    if (channel < 0 || channel >= 24 || !channels[channel].active) return;
    stopChannel(&channels[channel]);
}

bool replay(int channel) {
    if (channel < 0 || channel >= 24 || !channels[channel].active) return false;

    AudioChannel* ch = &channels[channel];

    // Pause playback
    ndspChnSetPaused(channel, true);

    // Reset/rewind vorbis file to beginning
    int res = ov_time_seek(&ch->vorbisFile, 0.0);
    if (res != 0) {
        print("Sound::replay: Failed to seek to start (error " + std::to_string(res) + ": " + vorbisStrError(res) + ")");
        ndspChnSetPaused(channel, false);
        return false;
    }

    // Free the previous buffer if it exists!
    if (ch->audioBuffer) {
        linearFree(ch->audioBuffer);
        ch->audioBuffer = nullptr;
    }

    // Clear all buffers to avoid old/half data
    ndspChnReset(channel);

    // Re-initialize audio data in buffers
    if (!audioInit(ch)) {
        print("Sound::replay: Failed to reinit audio buffer");
        ndspChnSetPaused(channel, false);
        return false;
    }

    // Resume playback
    ndspChnSetPaused(channel, false);

    return true;
}

void exit() {
    if (!system_initialized) return;

    // Stop all audio channels and threads first!
    for (int i = 0; i < 24; i++) {
        if (channels[i].active) {
            stopChannel(&channels[i]);
        }
    }

    system_initialized = false;
}

} // namespace Sound
} // namespace dsge

// Internal implementation
namespace {
    bool fillBuffer(AudioChannel* channel) {
        for (size_t i = 0; i < ARRAY_SIZE(channel->waveBufs); ++i) {
            if (channel->waveBufs[i].status != NDSP_WBUF_DONE) continue;
            
            int totalBytes = 0;
            while (static_cast<size_t>(totalBytes) < channel->waveBufs[i].nsamples * sizeof(s16)) {
                int16_t* buffer = channel->waveBufs[i].data_pcm16 + (totalBytes / sizeof(s16));
                const size_t bufferSize = (channel->waveBufs[i].nsamples * sizeof(s16) - totalBytes);

                const int bytesRead = ov_read(&channel->vorbisFile, (char*)buffer, bufferSize, nullptr);
                if (bytesRead <= 0) {
                    if (bytesRead == 0) break;
                    print("Channel " + std::to_string(channel->channel_id) + ": ov_read error " + std::to_string(bytesRead) + " (" + vorbisStrError(bytesRead) + ")");
                    break;
                }
                totalBytes += bytesRead;
            }

            if (totalBytes == 0) {
                return false;
            }

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
                break;
            }
            LightEvent_Wait(&s_event);
        }
        
        channel->active = false;
    }

    bool audioInit(AudioChannel* channel) {
        vorbis_info* vi = ov_info(&channel->vorbisFile, -1);
        
        ndspChnReset(channel->channel_id);
        ndspSetOutputMode(NDSP_OUTPUT_STEREO);
        ndspChnSetInterp(channel->channel_id, NDSP_INTERP_POLYPHASE);
        ndspChnSetRate(channel->channel_id, vi->rate);
        ndspChnSetFormat(channel->channel_id, vi->channels == 1 ? NDSP_FORMAT_MONO_PCM16 : NDSP_FORMAT_STEREO_PCM16);

        const size_t SAMPLES_PER_BUF = vi->rate * 120 / 1000;
        const size_t CHANNELS_PER_SAMPLE = vi->channels;
        const size_t WAVEBUF_SIZE = SAMPLES_PER_BUF * CHANNELS_PER_SAMPLE * sizeof(s16);
        const size_t bufferSize = WAVEBUF_SIZE * ARRAY_SIZE(channel->waveBufs);
        
        channel->audioBuffer = (int16_t*)linearAlloc(bufferSize);
        if (!channel->audioBuffer) {
            print("Failed to allocate audio buffer for channel " + channel->channel_id);
            return false;
        }

        memset(&channel->waveBufs, 0, sizeof(channel->waveBufs));
        int16_t* buffer = channel->audioBuffer;

        for (size_t i = 0; i < ARRAY_SIZE(channel->waveBufs); ++i) {
            channel->waveBufs[i].data_vaddr = buffer;
            channel->waveBufs[i].nsamples = WAVEBUF_SIZE / sizeof(buffer[0]);
            channel->waveBufs[i].status = NDSP_WBUF_DONE;
            buffer += WAVEBUF_SIZE / sizeof(buffer[0]);
        }

        return true;
    }

    void audioExit(AudioChannel* channel) {
        ndspChnReset(channel->channel_id);
        if (channel->audioBuffer) {
            linearFree(channel->audioBuffer);
            channel->audioBuffer = nullptr;
        }
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

    const char* vorbisStrError(int error) {
        switch(error) {
            case OV_FALSE: return "Request did not succeed";
            case OV_HOLE: return "Hole in page sequence numbers";
            case OV_EREAD: return "Read/seek/tell operation failed";
            case OV_EFAULT: return "Internal library error";
            case OV_EIMPL: return "Unimplemented feature";
            case OV_EINVAL: return "Invalid parameter";
            case OV_ENOTVORBIS: return "Not a valid Ogg Vorbis stream";
            case OV_EBADHEADER: return "Invalid header";
            case OV_EVERSION: return "Unrecognized version";
            case OV_EBADPACKET: return "Failed to decode packet";
            case OV_EBADLINK: return "Corrupt stream or lost data";
            case OV_ENOSEEK: return "Seek on unseekable stream";
            default: return "Unknown error";
        }
    }
}