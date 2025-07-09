/*
    DSGE Template
    First off, initialize dsge.hpp, if you don't have it then download the "dsge.hpp" in the root of this repo.
*/
#include "dsge.hpp"

// Template main function.
int main() {
    // Initialize DSGE, which initialize what dsge needs.
    dsge::init();

    // Using the basic PRINT to print out "Hello, World!" which gives the current line, file name and the output.
    print("Hello, World!");

    // Current loop.
    while (aptMainLoop()) {
        /*
            Checking if the key START is touched.
            Be careful, inside an dsge render function will throw an error!
        */
        hidScanInput();
        if (hidKeysDown() & KEY_START) {
            break;
        }

        // This is where you can render your stuff here (text, sprite, etc). 
        dsge::render([&]() {
            /*
                Your render sprites here.
                dsge::Sprite or dsge::Text must ends with .render(); to actually render!
            */
        });
    }

    /*
        Exits DSGE and all other services needed for DSGE.
        It will return 0 (meaning success).
    */
    return dsge::exit();
}