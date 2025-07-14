#include <string>
#include <3ds.h>

namespace dsge {

class Sound {
public:
    // Member variables as requested
    int length;         // Length of the audio in ms
    int time;           // Current playback position in ms
    float volume;       // Volume (0.0-1.0)
    bool loop;          // Whether to loop the sound

    /**
     * @brief Constructor - loads a sound file
     * @param path Path to the Ogg Vorbis file (romfs path)
     */
    Sound(const std::string& path);
    
    /**
     * @brief Destructor - cleans up resources
     */
    ~Sound();

    /**
     * @brief Plays the sound
     */
    void play();
    
    /**
     * @brief Pauses the sound
     */
    void pause();
    
    /**
     * @brief Resumes a paused sound
     */
    void resume();
    
    /**
     * @brief Stops the sound
     */
    void stop();
    
    /**
     * @brief Replays the sound from the beginning
     * @return true if successful, false otherwise
     */
    bool replay();

private:
    std::string filePath;
    int channel = -1;
    bool isPlaying = false;
    bool isPaused = false;
    
    // Internal functions
    void calculateLength();
};

} // namespace dsge