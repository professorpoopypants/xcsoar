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

#include "MapWindow/MapWindowStatusBar.hpp"
#include "MapButton/MainMenuButton.hpp"
#include "MapButton/LoggerStatusButton.hpp"
#include "MapButton/FlightModeButton.hpp"
#include "MapButton/FlarmStatusButton.hpp"
#include "MapButton/MapScaleButton.hpp"
#include "Screen/Layout.hpp"

class FlarmState;
class MapLook;


MapWindowStatusBar::MapWindowStatusBar()
{
  MainMenuButton* main_menu_button = new MainMenuButton();
  this->Add(main_menu_button);

  logger_status_button = new LoggerStatusButton();
  this->Add(logger_status_button);

  flight_mode_button = new FlightModeButton();
  this->Add(flight_mode_button);

  flarm_status_button = new FlarmStatusButton();
  this->Add(flarm_status_button);

  map_scale_button = new MapScaleButton();
  this->Add(map_scale_button);
}

void
MapWindowStatusBar::Draw(Canvas &canvas, const PixelRect &rc_map)
{
  const PixelScalar height = Layout::FastScale(16);
  const PixelScalar bottom_margin = 0;
  PixelRect psize;
  psize.left = 100;
  psize.right = 150;
  psize.top = rc_map.bottom - rc_map.top - height - bottom_margin;
  psize.bottom = psize.top + height;
  Move(psize);
  MapButtonPanel::Draw(canvas, rc_map);
}

void
MapWindowStatusBar::UpdateLogger(bool logger_status, uint8_t second)
{
  assert(logger_status_button);
  logger_status_button->Update(logger_status, second);
}

void
MapWindowStatusBar::UpdateFlightMode(bool task_abort,
                                       DisplayMode display_mode)
{
  flight_mode_button->Update(task_abort, display_mode);
}

void
MapWindowStatusBar::UpdateFlarmStatus(bool flarm_enabled,
                                        const FlarmState &flarm,
                                        const MapLook &look)
{
  flarm_status_button->Update(flarm_enabled, flarm, look);
}

void
MapWindowStatusBar::UpdateMapScale(fixed _map_width, const MapLook &_look,
                                   RasterWeather* _weather,
                                   bool _auto_zoom_enabled, bool _replay,
                                   bool _simulator)
{
  map_scale_button->Update(_map_width, _look,
                           _weather,
                           _auto_zoom_enabled, _replay,
                           _simulator);
}
