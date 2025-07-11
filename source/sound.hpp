#include <string>
#include <3ds.h>

namespace dsge {
namespace Sound {

// Initializes the Sound system
void init();

/**
 * @brief Plays a Sound file without stopping others
 * @note Make sure the audio file is in a format that the 3ds can play.
 * 
 * FFMPEG Command: `ffmpeg -i (sound here).ogg -c:a libvorbis -ar 44100 -ac 1 -b:a 96k output.ogg`
 * 
 * @param path Path to the Ogg Vorbis file (romfs path)
 * @return Channel number or -1 on error
 * 
 * #### Example Usage:
 * ```
 * // Playing a sound
 * dsge::Sound::play("test.ogg");
 * 
 * // It can be an integer to represent as a channel
 * int testChannel = dsge::Sound::play("test.ogg");
 * ```
 */
int play(const std::string& path);

/**
 * @brief Pauses a specific channel
 * @param channel Channel number to pause
 * 
 * #### Example Usage:
 * ```
 * // Playing and pausing a sound
 * int channel = dsge::Sound::play("test.ogg");
 * dsge::Sound::pause(channel);
 * ```
 */
void pause(int channel);

/**
 * @brief Resumes a paused channel
 * @param channel Channel number to resume
 * 
 * #### Example Usage:
 * ```
 * // Playing, pausing and resuming a sound
 * int channel = dsge::Sound::play("test.ogg");
 * dsge::Sound::pause(channel);
 * dsge::Sound::resume(channel);
 * ```
 */
void resume(int channel);

/**
 * @brief Replays a sound on an existing channel from the beginning, stutter-free.
 * @param channel Channel number to replay
 * @returns `true` if successful, `false` otherwise, see example usage for a demonstration for a real replay replica.
 * 
 * #### Example Usage:
 * ```
 * int channel = dsge::Sound::play("test.ogg");
 * dsge::Sound::replay(channel);
 * 
 * // Replaying a sound even if it doesn't exist anymore:
 * std::string sound = "sound.ogg";
 * channel = dsge::Sound::play("sound.ogg");
 * 
 * dsge::Timer::start(5, [&]() {
 *     if (!dsge::Sound::replay(channel)) {
 *         channel = dsge::Sound::play(sound);
 *     }
 * });
 * ```
 */
bool replay(int channel);

/**
 * @brief Stops a specific channel
 * @param channel Channel number to stop
 * 
 * #### Example Usage:
 * ```
 * // Playing and stopping a sound
 * int channel = dsge::Sound::play("test.ogg");
 * dsge::Sound::stop(channel);
 * ```
 */
void stop(int channel);

// Cleans up Sound system resources
void exit();

} // namespace Sound
} // namespace dsge