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

#ifndef XCSOAR_MAP_WINDOW_STATUS_BAR_HPP
#define XCSOAR_MAP_WINDOW_STATUS_BAR_HPP

#include "Screen/Point.hpp"
#include "MapButton/MapButtonPanel.hpp"
#include "DisplayMode.hpp"
#include "Math/fixed.hpp"

class Canvas;
class PixelRect;
class LoggerStatusButton;
class FlightModeButton;
class FlarmStatusButton;
class MapScaleButton;

class FlarmState;
class MapLook;
class RasterWeather;

/**
 * The set of visible buttons on the bottom of the map screen
 */
class MapWindowStatusBar : public MapButtonPanel {

protected:
  LoggerStatusButton* logger_status_button;
  FlightModeButton* flight_mode_button;
  FlarmStatusButton* flarm_status_button;
  MapScaleButton* map_scale_button;

public:
  MapWindowStatusBar();

  /**
    * Positions the panel at the map bottom TBD
    * Positions and draws each button;
    * @param canvas
    * @param rc_map
    */
   virtual void Draw(Canvas &canvas, const PixelRect &rc_map);

  /**
   * called by HandleMouseDown() if coordinates are
   * inside click_area
   */
  virtual void on_click() {};
  void UpdateLogger(bool logger_status, uint8_t second);
  void UpdateFlightMode(bool task_abort, DisplayMode display_mode);
  void UpdateFlarmStatus(bool flarm_enabled, const FlarmState &flarm,
                         const MapLook &look);
  void UpdateMapScale(fixed _map_width, const MapLook &_look,
                      RasterWeather* _weather,
                      bool _auto_zoom_enabled, bool _replay,
                      bool _simulator);

};

#endif
