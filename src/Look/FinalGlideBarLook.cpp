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

#include "FinalGlideBarLook.hpp"
#include "Screen/Layout.hpp"

void
FinalGlideBarLook::Initialise(const ColorTheme &color_theme)
{
  brush_below.Set(color_theme.red);
  brush_below_mc0.Set(LightColor(color_theme.red));
  pen_below.Set(Layout::ScalePenWidth(1), DarkColor(color_theme.red));

  brush_below_landable.Set(color_theme.orange);
  brush_below_landable_mc0.Set(LightColor(color_theme.orange));
  pen_below_landable.Set(Layout::ScalePenWidth(1),
                         DarkColor(color_theme.orange));

  brush_above.Set(color_theme.green);
  brush_above_mc0.Set(LightColor(LightColor(color_theme.green)));
  pen_above.Set(Layout::ScalePenWidth(1), DarkColor(color_theme.green));
}
