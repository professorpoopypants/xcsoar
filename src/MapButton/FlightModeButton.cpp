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

#include "MapButton/FlightModeButton.hpp"
#include "LogFile.hpp"
#include "Screen/Graphics.hpp"
#include "Screen/Icon.hpp"
#include "Screen/Layout.hpp"


FlightModeButton::FlightModeButton():
  MapButton(),
  task_abort(false),
  display_mode(DM_CIRCLING)
{
}

void
FlightModeButton::Draw(Canvas &canvas, const PixelRect &rc_map)
{
  // draw flight mode
  const MaskedIcon *bmp;

  if (task_abort)
    bmp = &Graphics::hAbort;
  else if (display_mode == DM_CIRCLING)
    bmp = &Graphics::hClimb;
  else if (display_mode == DM_FINAL_GLIDE)
    bmp = &Graphics::hFinalGlide;
  else
    bmp = &Graphics::hCruise;

  bmp->Draw(canvas, size.left,
            size.bottom - bmp->GetSize().cy - Layout::Scale(2));

  size.right = size.left + bmp->GetSize().cx;
}

void
FlightModeButton::Update(bool _task_abort, DisplayMode _display_mode)
{
  task_abort = _task_abort;
  display_mode = _display_mode;
}
