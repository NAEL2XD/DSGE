#include <3ds.h>
#include <string>
#include <string.h>

namespace dsge {
namespace Applet {
/**
 * @brief Sends a notification to 3ds's notification.
 * @param title The title to display to the top.
 * @param description The description to set as.
 * 
 * #### Example Usage:
 * ```
 * dsge::Applet::sendNotification("Hello, DSGE!", "Hello, World!");
 * ``` 
 */
void sendNotification(std::string title, std::string description);

/**
 * @brief Displays a error popup with custom text
 *     
 * @param errorCtx errorConf struct to use.
 * @param text Error message to display (ASCII).
 * 
 * #### errorCtx Usage:
 * ```
 * errorType type; // see errorType enum
 * int errorCode; // can be 0-9999999, if it's 1001111: 100-1111
 * bool homeButton; // Whatever or not you want to allow to press the home button
 * ```
 * 
 * #### Example Usage:
 * ```
 * errorConf error;
 * error.errorCode = 20102; // Will be 002-0102
 * error.homeButton = false; // Will not allow you to go to home menu
 * 
 * dsge::Applet::popupError(error, "You got an error!");
 * ```
 */
void popupError(errorConf errorCtx, std::string text);

/**
 *  @brief Reads the input text from user and returns the string result.
 *  @return String with text inputted to user.
 */
std::string swkbdGetInput();

/**
 * @brief Opens an URL using the 3DS's internet browser method
 * @param url The url to open as, can be nothing for `"https://example.com"`
 * 
 * #### Note:
 * 
 * This file is COPIED from this url: "https://gitlab.com/3ds-netpass/netpass/-/blob/main/source/utils.c?ref_type=heads#L395-L409"
 * 
 * #### Example Usage:
 * ```
 * dsge::openURL("https://example.com"); // Opens an url being "https://example.com"
 * dsge::openURL(); // Also opens an url being "https://example.com" since it's their defaults
 * ```
 */
void openURL(std::string url = "https://example.com");
}
}