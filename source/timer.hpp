#include <functional>

namespace dsge {
namespace Timer {

/**
 * @brief Starts a timer that calls a function after a delay
 * @param seconds Delay in seconds before triggering
 * @param callback Function to call when timer completes
 * @param loops Number of times to repeat (default 1)
 * 
 * #### Details:
 * 
 * The timer runs asynchronously without blocking the main thread.
 * 
 * The callback function will be called after the specified delay.
 * 
 * If loops > 1, the timer will repeat after each interval.
 * 
 * #### Example Code:
 * ```
 * // One-time timer
 * DSGE::TIMER::start(2.5f, []() {
 *     print("Timer done!");
 * });
 * 
 * // Repeating timer
 * DSGE::TIMER::start(1.0f, []() {
 *     print("Ping!");
 * }, 5); // Repeat 5 times
 * ```
 */
void start(float seconds, std::function<void()> callback, int loops = 1);

} // namespace TIMER
} // namespace DSGE