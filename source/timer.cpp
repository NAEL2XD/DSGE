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
            svcSleepThread(1000000000 * seconds);
            
            // Execute the callback
            callback();
        }
    }).detach();
}

} // namespace TIMER
} // namespace DSGE