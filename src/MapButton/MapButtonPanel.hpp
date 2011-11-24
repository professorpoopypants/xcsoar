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

#ifndef XCSOAR_MAP_BUTTON_PANEL_HPP
#define XCSOAR_MAP_BUTTON_PANEL_HPP

#include "Screen/Point.hpp"
#include "MapButton/MapButton.hpp"
#include "Util/StaticArray.hpp"


class Canvas;

/**
 * The set of visible buttons on the bottom of the map screen
 */
class MapButtonPanel {

protected:
  StaticArray<MapButton *, 8u> map_buttons;
  PixelRect size; // left side is not used., just top, bottom and right

public:
  ~MapButtonPanel();

  /**
   * Executes HandleMouseDown for each button
   * and executes first one that returns true
   * @param x mouse down coordinates
   * @param y
   * @return True if point in clickable_area of one of the buttons
   */
  bool HandleMouseDown(PixelScalar x, PixelScalar y);

  /**
   * Positions and draws each button, left to right.
   * Button[n] is positioned adjacent to button[n-1]
   * Widths of buttons are calculated which may change
   * during the state of the system
   * @param canvas
   * @param rc_map
   */
  virtual void Draw(Canvas &canvas, const PixelRect &rc_map);

  /**
   * Add a MapButton to the panel
   * Does not size or position the button
   * which is done as each button is drawn
   * @param map_button
   */
  void Add(MapButton *map_button);

  /**
   * returns next x relative not occupied by a button relative to map
   * @return x value relative to map
   */
  //PixelScalar NextFreeX();

  /**
   * repositions the upper left corner of the button panel
   */
  void Move(RasterPoint upper_left);

  /**
   * moves and sets height and width of panel
   * @param rc
   */
  void Move(PixelRect rc);
};

#endif
