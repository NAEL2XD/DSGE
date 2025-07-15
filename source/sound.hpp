#include <string>
#include <3ds.h>

namespace dsge {

/**
 * @class Sound
 * @brief Handles audio playback for Ogg Vorbis files on the Nintendo 3DS
 * 
 * This class provides a simple interface for loading and playing sound files,
 * with support for playback control, volume adjustment, and looping.
 * 
 * #### Supported Audio Formats:
 * - Ogg Vorbis (.ogg) with recommended settings:
 *   - Sample rate: 44100 Hz
 *   - Channels: 1 (mono) or 2 (stereo)
 *   - Bitrate: 96kbps
 * 
 * #### FFMPEG Conversion Command:
 * ```
 * ffmpeg -i input.ogg -c:a libvorbis -ar 44100 -ac 1 -b:a 96k output.ogg
 * ```
 */
class Sound {
public:
    /**
     * @brief Constructs a Sound object and loads the specified audio file
     * @param path Path to the Ogg Vorbis file in romfs (e.g., "sounds/effect.ogg")
     * 
     * #### FFMPEG Conversion Command:
     * 
     * `ffmpeg -i input.ogg -c:a libvorbis -ar 44100 -ac 1 -b:a 96k output.ogg`
     * 
     * #### Example Usage:
     * ```
     * // Load a sound from romfs:/sounds/effect.ogg
     * dsge::Sound effect("sounds/effect.ogg");
     * ```
     */
    Sound(const std::string& path);
    
    /**
     * @brief Destructor - automatically stops playback and cleans up resources
     */
    ~Sound();

    /**
     * @brief Starts playback of the sound
     * 
     * If the sound is already playing, this will resume playback if paused.
     * If the sound is at the end, it will restart from the beginning.
     * 
     * #### Example Usage:
     * ```
     * dsge::Sound bgm("music.ogg");
     * bgm.play();
     * ```
     */
    void play();
    
    /**
     * @brief Pauses the sound playback
     * 
     * The playback position is preserved and can be resumed with resume().
     * 
     * #### Example Usage:
     * ```
     * sound.pause();
     * // Later...
     * sound.resume();
     * ```
     */
    void pause();
    
    /**
     * @brief Resumes playback from a paused state
     * 
     * #### Example Usage:
     * ```
     * sound.pause();
     * // Later...
     * sound.resume();
     * ```
     */
    void resume();
    
    /**
     * @brief Stops playback and resets to the beginning
     * 
     * #### Example Usage:
     * ```
     * sound.play();
     * // Later...
     * sound.stop(); // Playback position is reset to 0
     * ```
     */
    void stop();
    
    /**
     * @brief Restarts playback from the beginning
     * @return true if successful, false if playback couldn't be restarted
     * 
     * This provides a smooth restart without audio glitches.
     * 
     * #### Example Usage:
     * ```
     * sound.play();
     * // When you need to restart:
     * if (!sound.replay()) {
     *     sound.play(); // Fallback if replay fails
     * }
     * ```
     */
    bool replay();

    // Public member variables
    int length;         ///< Total duration of the sound in milliseconds (read-only)
    int time;           ///< Current playback position in milliseconds (read-only)
    float volume;       ///< Playback volume (0.0 = silent, 1.0 = full volume)
    bool loop;          ///< Whether the sound should loop automatically (default: false)

private:
    std::string filePath;
    int channel;
    bool isPlaying;
    bool isPaused;
    
    void calculateLength();
};

} // namespace dsge