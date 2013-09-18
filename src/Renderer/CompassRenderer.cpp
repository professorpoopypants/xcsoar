/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2013 The XCSoar Project
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

#include "CompassRenderer.hpp"
#include "Look/MapLook.hpp"
#include "Screen/Canvas.hpp"
#include "Screen/Layout.hpp"
#include "Math/Angle.hpp"
#include "Math/Screen.hpp"
#include "Util/Macros.hpp"

void
CompassRenderer::Draw(Canvas &canvas, const Angle screen_angle,
                      const PixelRect rc)
{
  RasterPoint pos;
  pos.y = Layout::Scale(19) + rc.top;
  pos.x = rc.right - Layout::Scale(19);

  Draw(canvas, screen_angle, pos);
}

void
CompassRenderer::Draw(Canvas &canvas, const Angle screen_angle,
                      const RasterPoint pos)
{
  RasterPoint arrow[5] = { { 0, -13 }, { -6, 10 }, { 0, 4 }, { 6, 10 }, { 0, -13 } };

  canvas.Select(look.compass_pen);
  canvas.Select(look.compass_brush);

  // North arrow
  PolygonRotateShift(arrow, ARRAY_SIZE(arrow), pos.x, pos.y, -screen_angle);
  canvas.DrawPolygon(arrow, ARRAY_SIZE(arrow));

  canvas.Select(look.compass_triangle_pen);
  canvas.Select(look.compass_triangle_brush);

  RasterPoint black_triangle[4] = { { 0, -13 }, { 6, 10}, { 0, 4}, { 0, -13 } };
  PolygonRotateShift(black_triangle, ARRAY_SIZE(black_triangle),
                     pos.x, pos.y, -screen_angle);
  canvas.DrawPolygon(black_triangle, ARRAY_SIZE(black_triangle));
}
