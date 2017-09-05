// @todo - really make a part of the test suite. Right now just designed
// to be a standalone test
//
// Right now to build:
// g++ -Isrc/OS test/src/UInput.cpp src/OS/UInput.cpp

#include <unistd.h> // sleep

//#include "tap.h" // Get link failure, missing tap.o

#include "OS/UInput.hpp"


int main(int argc, char **argv)
{
    UInput uinput;

    usleep(100000); // wait for device to be registered by window manager / wayland
    uinput.SendKey('h');
    uinput.SendKey('e');
    uinput.SendKey('l');
    uinput.SendKey('l');
    uinput.SendKey('o');
    uinput.SendKey('1');
    uinput.SendKey('2');
    uinput.SendKey('3');
    uinput.SendKey('\r');
    uinput.SendKey('\n');
    usleep(100);
    // ok(true, "uinput"); // see include tap.h comment
}
