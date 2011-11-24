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

#ifndef XCSOAR_LOGGER_STATUS_BUTTON_HPP
#define XCSOAR_LOGGER_STATUS_BUTTON_HPP

#include "MapButton/MapButton.hpp"
#include <stdint.h>

class canvas;
class PixelRect;

class LoggerStatusButton : public MapButton {
protected:
  bool logger_status;
  uint8_t second;

public:
  LoggerStatusButton();

  /**
   * draws logger status icon, flashing
   * @param canvas
   * @param rc
   */
  virtual void Draw(Canvas &canvas, const PixelRect &rc);

  /**
   * Updates data needed to draw current status
   * @param _logger_status
   * @param _second
   */
  void Update(bool _logger_status, uint8_t _second);
};

#endif
