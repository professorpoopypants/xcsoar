/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
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

#include "Android/Nook.hpp"
#include "PeriodClock.hpp"

#include <stdlib.h>

static char cmd_sleep[] = "sleep 1";
static char cmd_host[] = "su -c 'echo host > /sys/devices/platform/musb_hdrc/mode'";
static char cmd_peripheral[] = "su -c 'echo peripheral > /sys/devices/platform/musb_hdrc/mode'";
static char cmd_usb_rw[] = "su -c 'chmod 666 /dev/ttyUSB0'";
static char cmd_usb_ro[] = "su -c 'chmod 500 /dev/ttyUSB0'";
static char cmd_init_date[] = "date > /sdcard/initusbdate";
static char cmd_deinit_date[] = "date > /sdcard/deinitusbdate";
static char cmd_set_charge_500[] = "su -c 'echo 500000 > /sys/devices/platform/bq24073/regulator/regulator.5/device/force_current'";
static PeriodClock init_clock;
static PeriodClock deinit_clock;

/**
 * sets max charge to 500mA.  any higher breaks the USB
 */
static void
SetCharge500()
{
  system(cmd_set_charge_500);
}

void
Nook::InitUsb()
{
  if (!init_clock.CheckUpdate(5000))
    return;

  SetCharge500();

  system(cmd_host);
  system(cmd_sleep);

  system(cmd_host);
  system(cmd_sleep);

  system(cmd_usb_rw);
  system(cmd_sleep);
  system(cmd_usb_rw);

  system(cmd_init_date);

  // next to to InitUsb() should go through
  deinit_clock.Reset();
}

void
Nook::DeinitUsb()
{
  if (!deinit_clock.CheckUpdate(5000))
    return;

  system(cmd_peripheral);
  system(cmd_sleep);

  system(cmd_peripheral);
  system(cmd_sleep);

  system(cmd_usb_ro);
  system(cmd_deinit_date);

  // next to to InitUsb() should go through
  init_clock.Reset();
}
