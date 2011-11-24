/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2011 The XCSoar Project
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

#include "MapButton/FlarmStatusButton.hpp"
#include "Screen/Graphics.hpp"
#include "Screen/Icon.hpp"
#include "Screen/Layout.hpp"
#include "FLARM/State.hpp"
#include "Look/MapLook.hpp"

#include "LogFile.hpp" //debug

FlarmStatusButton::FlarmStatusButton():
  MapButton(),
  flarm_enabled(false),
  flarm(NULL),
  look(NULL)
{
}

void
FlarmStatusButton::Draw(Canvas &canvas, const PixelRect &rc_map)
{
  const MaskedIcon *bmp;

  if (!flarm_enabled) {
    // Don't show indicator when the gauge is indicating the traffic anyway
    size.right = size.left;
    return;
  }

  if (!flarm->available || flarm->GetActiveTrafficCount() == 0)
    return;

  switch (flarm->alarm_level) {
  case 0:
    bmp = &look->hBmpTrafficSafe;
    break;
  case 1:
    bmp = &look->hBmpTrafficWarning;
    break;
  case 2:
  case 3:
    bmp = &look->hBmpTrafficAlarm;
    break;
  };

  bmp->Draw(canvas, size.left,
            size.bottom - bmp->GetSize().cy - Layout::Scale(2));

  size.right = size.left + bmp->GetSize().cx + Layout::Scale(6);
}

void
FlarmStatusButton::Update(bool _flarm_enabled, const FlarmState &_flarm,
                          const MapLook &_look)
{
  flarm_enabled = _flarm_enabled;
  flarm = &_flarm;
  look = &_look;
}
