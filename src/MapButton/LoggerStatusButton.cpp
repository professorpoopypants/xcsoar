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

#include "MapButton/LoggerStatusButton.hpp"
#include "LogFile.hpp"
#include "Screen/Graphics.hpp"
#include "Screen/Icon.hpp"


LoggerStatusButton::LoggerStatusButton():
  MapButton(),
  logger_status(false),
  second(0)
{
}

void
LoggerStatusButton::Draw(Canvas &canvas, const PixelRect &rc_map)
{
  if (logger_status) {
    bool flip = (second % 2) == 0;
    MaskedIcon &icon = flip ? Graphics::hLogger : Graphics::hLoggerOff;
    size.right = size.left + icon.GetSize().cx;
    icon.Draw(canvas, size.left + margin, size.bottom - icon.GetSize().cy);
  } else {
    size.right = size.left;
  }
}

void
LoggerStatusButton::Update(bool _logger_status, uint8_t _second)
{
  logger_status = _logger_status;
  second = _second;
}
