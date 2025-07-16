#ifndef DSGE_APPLET_HPP
#define DSGE_APPLET_HPP

#include "dsge.hpp"
#include <string>

namespace dsge {
namespace Applet {
/**
 * @brief Sends a notification to the Applet and makes a blinking blue LED.
 * @param title The title to display to the notification, maximum 96 characters.
 * @param description The description to display as the information, maximum 2048 characters.
 * 
 * #### Example Usage:
 * ```
 * dsge::Applet::sendNotification("Hello, DSGE!", "Hello, World!");
 * ``` 
 */
void sendNotification(std::string title, std::string description);

/**
 * @brief Displays a error popup with custom text and customizable context.
 *     
 * @param errorCtx Styled configuration of error to use.
 * @param text Error message to display as the big text box.
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
 * @brief Reads the input text from the user and returns the result from the SWKBD.
 * 
 * This will use the SWKBD applet to launch the text and then return string inputted from the user.
 * 
 * @param hintText The hint name of the text you want to use as, leave nothing for `"Enter Text Here."`.
 * @return String with text inputted to user.
 * 
 * #### Example Usage:
 * ```
 * std::string newInput = dsge::Applet::swkbdGetInput();
 * 
 * print(newInput);
 * ```
 */
std::string swkbdGetInput(std::string hintText = "Enter Text Here.");

/**
 * @brief Opens an URL using the 3DS's internet browser method.
 * @param url The url to open as, can be nothing for `"https://example.com"`
 * 
 * #### Note:
 * 
 * This file is COPIED from this url: "https://gitlab.com/3ds-netpass/netpass/-/blob/main/source/utils.c?ref_type=heads#L395-L409"
 * 
 * #### Example Usage:
 * ```
 * dsge::openURL("https://example.com"); // Opens an url being "https://example.com"
 * dsge::openURL(); // Also opens an url being "https://example.com" since it's their default.
 * ```
 */
void openURL(std::string url = "https://example.com");
}
}

#endif