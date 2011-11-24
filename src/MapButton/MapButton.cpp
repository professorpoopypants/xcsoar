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

#include "MapButton/MapButton.hpp"
#include "Screen/Layout.hpp"
#include <winuser.h> // for PtInRect


MapButton::MapButton():
  call_back(NULL)
{
  margin = Layout::Scale(2);
  size_clickable.right = size_clickable.left =
      size_clickable.top = size_clickable.bottom = 0;
  size.right = size.left = size.top = size.bottom = 0;
}

MapButton::MapButton(ClickNotifyCallback_t cb):
  call_back(cb)
{
  margin = Layout::Scale(2);
}


bool
MapButton::HandleMouseDown(PixelScalar x, PixelScalar y)
{
  RasterPoint pos;
  pos.x = x;
  pos.y = y;

  if (!PtInRect(&size_clickable, pos))
    return false;

  return on_click();
}

void
MapButton::Move(PixelScalar x, PixelScalar y)
{
  RasterPoint delta;
  delta.x = x - size.left;
  delta.y = y - size.top;

  size.left += delta.x;
  size.right += delta.x;
  size.top += delta.y;
  size.bottom += delta.y;

  size_clickable.left += delta.x;
  size_clickable.right += delta.x;
  size_clickable.top += delta.y;
  size_clickable.bottom += delta.y;
}

void
MapButton::Resize(PixelScalar width, PixelScalar height)
{
  size_clickable.right = size.right = size.left + width;
  size_clickable.bottom = size.bottom = size.top + height;
}

bool
MapButton::on_click()
{
  if (call_back != NULL) {
    call_back();
    return true;
  }
  return false;
}
