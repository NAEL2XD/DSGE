/*
    DSGE Template
    First off, initialize dsge.hpp, if you don't have it then run the "template.py" in the root of this repo.
*/
#include "dsge/dsge.hpp"

// Template main function.
int main() {
    // Initialize DSGE, which initialize what dsge needs.
    dsge::init();

    // Using the basic TRACE to create a text being "Hello, World!" which gives the current line, file name and the output.
    trace("Hello, World!");

    // Current loop, always render first.
    while (dsge::render()) {
        // Checking if the key START is touched.
        hidScanInput();
        if (hidKeysDown() & KEY_START) {
            break;
        }
    }

    /*
        Exits DSGE and all other services needed for DSGE.
        It will return 0 (meaning success).
    */
    return dsge::exit();
}