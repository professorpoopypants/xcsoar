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

#ifndef XCSOAR_MAP_BUTTON_HPP
#define XCSOAR_MAP_BUTTON_HPP

#include "Screen/Point.hpp"

class Canvas;

/**
 * Abstract custom draw button class that can be drawn as an overlay
 * on the map
 * The size of the buttons is adjusted their custom draw() routines
 * which will vary depending on fonts and xcsoar status
 */
class MapButton {
public:
  typedef void (*ClickNotifyCallback_t)();

protected:
  PixelRect size;
  // the click region may be larger or smaller than the visible region
  PixelRect size_clickable;
  ClickNotifyCallback_t call_back;
  PixelScalar margin;


public:
  MapButton();

  MapButton(ClickNotifyCallback_t cb);

  /**
   * Checks mouse coordinates and executes on_click() if inside
   * @param x mouse down coordinates
   * @param y
   * @return True if point in clickable_area
   */
  bool HandleMouseDown(PixelScalar x, PixelScalar y);

  /**
   * @return size of the clickable region around the button
   * as determined by the latest Draw()
   * and Move()
   */
  const PixelRect& GetRectClickable() { return size_clickable; }

  /**
   * @return size of the button as determined by the latest Draw()
   * and Move()
   */
  const PixelRect& GetRect() { return size; }

  /**
   * Draws the button.
   * Assumes upper left corner of button is at size.left and size.top
   * Sets size.right and size.bottom based on size required to draw button
   * @param canvas
   * @param rc_map
   */
  virtual void Draw(Canvas &canvas, const PixelRect &rc_map) = 0;

  /**
   * positions the button's upper left corner on the map
   * adjust clickable region by same vector
   * @param pos
   */
  void Move(PixelScalar x, PixelScalar y);

  /**
   * Sets the width and height of the button
   * Sets the clickable region to the same size
   * @param width of new button
   * @param height
   */
  void Resize(PixelScalar width, PixelScalar height);

  /**
   * returns height of button
   * @return
   */
  PixelScalar Height() { return size.bottom - size.top; }

  /**
   * returns height of button
   * @return
   */
  PixelScalar Width() { return size.right - size.left; }

  /**
   * called by HandleMouseDown() if coordinates are
   * inside clickable_area
   * @return: True if callback exists
   */
  virtual bool on_click();
};

#endif
