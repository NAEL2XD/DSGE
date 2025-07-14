#include "applet.hpp"

namespace dsge {
namespace Applet {
void sendNotification(std::string title, std::string description) {
    // Convert strings to UTF-16
    u16 _title[96] = {0};
    u16 _message[2048] = {0};
    
    // Convert ASCII to UTF-16
    const char* ascii_title = title.c_str();
    const char* ascii_message = description.c_str();
    
    for (size_t i = 0; i < strlen(ascii_title); i++) {
        _title[i] = ascii_title[i];
    }
    
    for (size_t i = 0; i < strlen(ascii_message); i++) {
        _message[i] = ascii_message[i];
    }

    // Create notification
    NEWS_AddNotification(_title, strlen(ascii_title), _message, strlen(ascii_message), NULL, 0, false);
}

void popupError(errorConf errorCtx, std::string text) {
    errorInit(&errorCtx, ERROR_TEXT, CFG_LANGUAGE_EN);
    errorText(&errorCtx, text.c_str());
    errorCtx.homeButton = false;
    errorDisp(&errorCtx);
}

std::string swkbdGetInput(std::string hintText) {
    static char text[1024];
    SwkbdState swkbd;

    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, -1);
	swkbdSetFeatures(&swkbd, SWKBD_MULTILINE | SWKBD_DARKEN_TOP_SCREEN | SWKBD_FIXED_WIDTH);
	swkbdSetHintText(&swkbd, hintText.c_str());
    swkbdSetInitialText(&swkbd, text);
	swkbdInputText(&swkbd, text, sizeof(text));

    return (std::string)text;
}

void openURL(std::string url) {
    if (url == "NULL") {
        aptLaunchSystemApplet(APPID_WEB, 0, 0, 0);
        return;
    }
    size_t url_len = strlen(url.c_str()) + 1;
    if (url_len > 0x400) return openURL("NULL");
    size_t buffer_size = url_len + 1;
    u8* buffer = (u8*)malloc(buffer_size); // Add explicit cast here
    if (!buffer) return openURL("NULL");
    memcpy(buffer, url.c_str(), url_len);
    buffer[url_len] = 0;
    aptLaunchSystemApplet(APPID_WEB, buffer, buffer_size, 0);
    free(buffer);
}
}
}