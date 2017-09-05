#include <unistd.h> // usleep

#include "NMEA/Info.hpp"
#include "Device/Parser.hpp"
#include "Device/Driver/FakeKeyboard.hpp"

void TestFakeKeyboard()
{
    NMEAParser parser;

    NMEAInfo nmea_info;

    usleep(100000); // to let uinput be registered with window manager - test is not automated, we rely on a human visually seeing the chars
    parser.ParseLine("$FAKBD,h", nmea_info);
    parser.ParseLine("$FAKBD,e", nmea_info);
    parser.ParseLine("$FAKBD,l", nmea_info);
    parser.ParseLine("$FAKBD,l", nmea_info);
    parser.ParseLine("$FAKBD,o", nmea_info);
    parser.ParseLine("$FAKBD,1", nmea_info);
    parser.ParseLine("$FAKBD,2", nmea_info);
    parser.ParseLine("$FAKBD,3", nmea_info);
    parser.ParseLine("$FAKBD,\n", nmea_info);
    parser.ParseLine("$FAKBD,x", nmea_info);
    parser.ParseLine("$FAKBD,\n", nmea_info);
    usleep(100);
}

int main(int argc, char **argv)
{
    TestFakeKeyboard();
}
