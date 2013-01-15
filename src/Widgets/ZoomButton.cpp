/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
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

#include "ZoomButton.hpp"
#include "Screen/Layout.hpp"
#include "Look/InfoBoxLook.hpp"
#include "Look/IconLook.hpp"
#include "Look/DialogLook.hpp"
#include "Screen/Canvas.hpp"
#include "Asset.hpp"

void
ZoomButton::OnPaint(Canvas &canvas)
{
  PixelRect rc = {
    PixelScalar(0), PixelScalar(0), PixelScalar(canvas.get_width()),
    PixelScalar(canvas.get_height())
  };
  bool pressed = is_down();

  renderer.DrawButton(canvas, rc, HasFocus(), pressed, true);
  rc = renderer.GetDrawingRect(rc, pressed);
  UPixelScalar pen_width = IsGrayScaleScreen() ? 2 : 1;
  canvas.Select(Pen((UPixelScalar)Layout::Scale(pen_width), COLOR_BLACK));
  canvas.DrawTwoLines(rc.left, rc.bottom, rc.right, rc.bottom,
                      rc.right, rc.top);
  canvas.DrawTwoLines(rc.left, rc.bottom, rc.left, rc.top,
                      rc.right, rc.top);

  canvas.SelectNullPen();
  if (!IsEnabled())
    canvas.Select(look.button.disabled.brush);
  else
    canvas.Select(look.button.standard.foreground_brush);
  const Bitmap *bmp = zoom_in ? &icon_look.hBmpZoomInButton : &icon_look.hBmpZoomOutButton;
  const PixelSize bitmap_size = bmp->GetSize();
  const int offsetx = (rc.right - rc.left - bitmap_size.cx / 2) / 2;
  const int offsety = (rc.bottom - rc.top - bitmap_size.cy) / 2;
  canvas.CopyAnd(rc.left + offsetx,
                  rc.top + offsety,
                  bitmap_size.cx / 2,
                  bitmap_size.cy,
                  *bmp,
                  bitmap_size.cx / 2, 0);
}