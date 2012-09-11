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

#include "Look/AirspaceLook.hpp"
#include "Screen/Layout.hpp"
#include "Renderer/AirspaceRendererSettings.hpp"
#include "resource.h"
#include "Util/Macros.hpp"

#ifdef USE_GDI
#include "Screen/GDI/AlphaBlend.hpp"
#endif

Color AirspaceLook::preset_colors_color[];
Color AirspaceLook::preset_colors_gray[];

const Color*
AirspaceLook::GetPresetColors()
{
  if (HasColors())
    return preset_colors_color;
  else
    return preset_colors_gray;
}

void
AirspaceLook::Initialise(const AirspaceRendererSettings &settings,
                         const ColorTheme &color_theme)
{
  preset_colors_color[0] = color_theme.red;
  preset_colors_color[1] = color_theme.green;
  preset_colors_color[2] = color_theme.blue;
  preset_colors_color[3] = color_theme.yellow;
  preset_colors_color[4] = color_theme.magenta;
  preset_colors_color[5] = color_theme.cyan;
  preset_colors_color[6] = DarkColor(color_theme.red);
  preset_colors_color[7] = DarkColor(color_theme.green);
  preset_colors_color[8] = DarkColor(color_theme.blue);
  preset_colors_color[9] = DarkColor(color_theme.yellow);
  preset_colors_color[10] = DarkColor(color_theme.magenta);
  preset_colors_color[11] = DarkColor(color_theme.cyan);
  preset_colors_color[12] = COLOR_WHITE;
  preset_colors_color[13] = color_theme.light_gray;
  preset_colors_color[14] = color_theme.gray;
  preset_colors_color[15] = COLOR_BLACK;

  preset_colors_gray[0] = COLOR_WHITE;
  preset_colors_gray[1] = Color(255,255,214);
  preset_colors_gray[2] = Color(255,255,205);
  preset_colors_gray[3] = Color(255,255,204);
  preset_colors_gray[4] = Color(255,255,203);
  preset_colors_gray[5] = Color(255,255,202);
  preset_colors_gray[6] = Color(255,255,201);
  preset_colors_gray[7] = Color(255,255,200);
  preset_colors_gray[8] = COLOR_BLACK;


  for (unsigned i = 0; i < AIRSPACECLASSCOUNT; ++i) {
    const AirspaceClassRendererSettings &class_settings = settings.classes[i];

    if (class_settings.border_width != 0)
      pens[i].Set(Layout::ScalePenWidth(class_settings.border_width),
                  class_settings.border_color);

#ifdef HAVE_ALPHA_BLEND
    if (AlphaBlendAvailable())
#endif
#if defined(HAVE_ALPHA_BLEND) || !defined(HAVE_HATCHED_BRUSH)
      solid_brushes[i].Set(class_settings.fill_color);
#endif
  }

  // airspace brushes and colors
#ifdef HAVE_HATCHED_BRUSH
  bitmaps[0].Load(IDB_AIRSPACE0);
  bitmaps[1].Load(IDB_AIRSPACE1);
  bitmaps[2].Load(IDB_AIRSPACE2);
  bitmaps[3].Load(IDB_AIRSPACE3);
  bitmaps[4].Load(IDB_AIRSPACE4);
  bitmaps[5].Load(IDB_AIRSPACE5);
  bitmaps[6].Load(IDB_AIRSPACE6);
  bitmaps[7].Load(IDB_AIRSPACE7);

  for (unsigned i = 0; i < ARRAY_SIZE(AirspaceLook::brushes); i++)
    brushes[i].Set(bitmaps[i]);
#endif

  thick_pen.Set(Layout::ScalePenWidth(10), COLOR_BLACK);

  intercept_icon.Load(IDB_AIRSPACEI, IDB_AIRSPACEI_HD);
}
