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

#include "MapButton/MapButtonPanel.hpp"
#include "Screen/Layout.hpp"

#include "LogFile.hpp" //debug

MapButtonPanel::~MapButtonPanel()
{
  for (auto it = map_buttons.begin(), end = map_buttons.end(); it != end; ++it)
    delete *it;
}

bool
MapButtonPanel::HandleMouseDown(PixelScalar x, PixelScalar y)
{
  LogStartUp(_T("MapButtonPanel::HandleMouseDown x:%d y:%d"), x, y);
  for (auto it = map_buttons.begin(), end = map_buttons.end(); it != end; ++it) {
    if ((*it)->HandleMouseDown(x, y))
      return true;
  }
  return false;
}

void
MapButtonPanel::Draw(Canvas &canvas, const PixelRect &rc_map)
{
  PixelScalar offset = 0;
  const PixelScalar margin = 2;
  for (auto it = map_buttons.begin(), end = map_buttons.end(); it != end; ++it) {
    RasterPoint up_left;
    up_left.x = size.left + offset;
    up_left.y = size.top;
    (*it)->Move(up_left.x, up_left.y);
    PixelSize ps_empty;
    ps_empty.cx = 0;
    ps_empty.cy = size.bottom - size.top;
    (*it)->Resize(ps_empty.cx, ps_empty.cy);
    (*it)->Draw(canvas, rc_map);
    // if the button is not zero width (hidden) adjust for next button
    if (((*it)->GetRect().right - (*it)->GetRect().left) > 0)
      offset += ((*it)->GetRect().right - (*it)->GetRect().left) +
        Layout::Scale(margin);
  }
}
/*
PixelScalar
MapButtonPanel::NextFreeX()
{
  if (map_buttons.size() == 0)
    return size.left;

  return map_buttons[map_buttons.size() - 1]->GetRect().right + Layout::FastScale(2);
}
*/
void
MapButtonPanel::Add(MapButton *map_button)
{
  map_buttons.append(map_button);
}

void
MapButtonPanel::Move(RasterPoint upper_left)
{
  LogStartUp(_T("MapButtonPanel::Move x:%d y:%d"), upper_left.x, upper_left.y);
  PixelSize old;
  old.cx = size.right - size.left;
  old.cy = size.bottom - size.top;
  size.left = upper_left.x;
  size.top = upper_left.y;
  size.right = size.left + old.cx;
  size.bottom = size.top + old.cy;
}

void
MapButtonPanel::Move(PixelRect rc)
{
  size = rc;
}
