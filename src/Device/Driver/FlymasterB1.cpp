/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2013 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Device/Driver/FlymasterB1.hpp"
#include "Device/Driver.hpp"
#include "Device/Parser.hpp"
#include "Device/Util/NMEAWriter.hpp"
#include "NMEA/Checksum.hpp"
#include "NMEA/Info.hpp"
#include "NMEA/InputLine.hpp"
#include "Atmosphere/Temperature.hpp"

#include <stdlib.h>
#include <math.h>

class FlymasterB1Device : public AbstractDevice {
  Port &port;

public:
  FlymasterB1Device(Port &_port):port(_port) {}

  bool EnableNMEA(OperationEnvironment &env) override;
  bool ParseNMEA(const char *line, struct NMEAInfo &info) override;
};

bool
FlymasterB1Device::EnableNMEA(OperationEnvironment &env)
{
  /* this command initiates NMEA mode according to the "Flymaster B1
     Commands" document */
  return PortWriteNMEA(port, "$PFMNAV,", env);
}

static bool
VARIO(NMEAInputLine &line, NMEAInfo &info)
{
  // $VARIO,fPressure,fVario,Bat1Volts,Bat2Volts,BatBank,TempSensor1,TempSensor2*CS

  // fVario = the variometer in decimeters per second
  // Bat1Volts = the voltage of the battery in bank 1
  // Bat2Volts = the voltage of the battery in bank 2
  // BatBank = the battery bank in use.
  // TempSensor1 = temperature in ºC of external wireless sensor 1
  // TempSensor2 = temperature in ºC of external wireless sensor 2

  fixed value;
  if (line.ReadChecked(value))
    info.ProvideStaticPressure(AtmosphericPressure::Pascal(value));

  if (line.ReadChecked(value))
    info.ProvideTotalEnergyVario(value / 10);

  unsigned battery_bank;
  fixed voltage[2];
  if (line.ReadChecked(voltage[0]) &&
      line.ReadChecked(voltage[1]) &&
      line.ReadChecked(battery_bank) &&
      battery_bank != 0 &&
      battery_bank <= 2) {
    info.voltage = voltage[battery_bank - 1];
    info.voltage_available.Update(info.clock);
  }

  if (line.ReadChecked(value)) {
    info.temperature = CelsiusToKelvin(value);
    info.temperature_available = true;
  }

  return true;
}

bool
FlymasterB1Device::ParseNMEA(const char *String, NMEAInfo &info)
{
  if (!VerifyNMEAChecksum(String))
    return false;

  NMEAInputLine line(String);
  char type[16];
  line.Read(type, 16);

  if (StringIsEqual(type, "$VARIO"))
    return VARIO(line, info);
  else
    return false;
}

static Device *
FlymasterB1CreateOnPort(const DeviceConfig &config, Port &port)
{
  return new FlymasterB1Device(port);
}

const struct DeviceRegister flymaster_b1_driver = {
  _T("FlymasterB1"),
  _T("Flymaster B1"),
  0,
  FlymasterB1CreateOnPort,
};
