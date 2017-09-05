#include <unistd.h> // usleep

#include "NMEA/Info.hpp"
#include "Device/Driver.hpp"
#include "Device/Config.hpp"
#include "Device/Port/NullPort.hpp"
#include "Device/Parser.hpp"
#include "Device/Driver/FakeKeyboard.hpp"

void TestFakeKeyboard()
{
    const DeviceConfig dummy_config = DeviceConfig();
    NullPort null;
    Device *device = fakekeyboard_driver.CreateOnPort(dummy_config, null);
    if (device == NULL)
        return;

    NMEAInfo nmea_info;
    nmea_info.Reset();
    nmea_info.clock = fixed(1);
    nmea_info.alive.Update(nmea_info.clock);

    usleep(100000); // to let uinput be registered with window manager - test is not automated, we rely on a human visually seeing the chars
    device->ParseNMEA("$FAKBD,h*E", nmea_info);
    device->ParseNMEA("$FAKBD,e", nmea_info);
    device->ParseNMEA("$FAKBD,l", nmea_info);
    device->ParseNMEA("$FAKBD,l", nmea_info);
    device->ParseNMEA("$FAKBD,o", nmea_info);
    device->ParseNMEA("$FAKBD,1", nmea_info);
    device->ParseNMEA("$FAKBD,2", nmea_info);
    device->ParseNMEA("$FAKBD,3", nmea_info);
    device->ParseNMEA("$FAKBD,\n", nmea_info);
    device->ParseNMEA("$FAKBD,x", nmea_info);
    device->ParseNMEA("$FAKBD,\n", nmea_info);
    usleep(100);
}

int main(int argc, char **argv)
{
    TestFakeKeyboard();
}
