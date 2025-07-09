#include "timer.hpp"
#include "3ds/svc.h"
#include <thread>

namespace dsge {
namespace Timer {

void start(float seconds, std::function<void()> callback, int loops) {
    if (loops <= 0) {
        loops = 2147483647;
    }

    std::thread([seconds, callback, loops]() {
        using namespace std::chrono;
        
        for (int i = 0; i < loops; i++) {
            // Sleep for the specified duration
            svcSleepThread(100000000 * seconds);
            
            // Execute the callback
            callback();
            
            // Early exit if we're asked to stop during sleep
            if (loops < 0) break;
        }
    }).detach();
}

} // namespace TIMER
} // namespace DSGE