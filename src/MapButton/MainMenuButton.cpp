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

#include "MapButton/MainMenuButton.hpp"
#include "Screen/Canvas.hpp"
#include "Language/Language.hpp"
#include "Screen/Fonts.hpp"
#include "Screen/Layout.hpp"
#include "InputEvents.hpp"

MainMenuButton::MainMenuButton():
  MapButton(InputEvents::ShowMenu)
{
  help_line1 = gettext(_T("Click on the \"M\" on the bottom"));
  help_line2 = gettext(_T("of the screen to show the menu."));
  m_text = _T(" M ");

  clock.update();
};

void
MainMenuButton::Draw(Canvas &canvas, const PixelRect &rc_map)
{
  const PixelScalar margin = Layout::FastScale(2);
  canvas.select(Fonts::MapBold);
  PixelSize tsize = canvas.text_size(m_text);

  Resize(tsize.cx + 2 * margin, tsize.cy + 2 * margin);

  // make click area larger than button to avoid accidental map click
  size_clickable.left = size.left - Layout::FastScale(5);
  size_clickable.right = size.right + Layout::FastScale(5);
  size_clickable.top = size.top - Layout::FastScale(7);

  canvas.black_pen();
  canvas.white_brush();
  canvas.round_rectangle(size.left, size.top,
                         size.right, size.bottom,
                         Layout::Scale(8), Layout::Scale(8));

  canvas.background_transparent();
  canvas.set_text_color(COLOR_BLACK);

  canvas.text(size.left + margin, size.top + margin, m_text);
  canvas.background_opaque();
}

void
MainMenuButton::DrawMenuButtonHelp(Canvas &canvas,
                                   const PixelRect &rc_map) const
{
  if (!InputEvents::IsMenuNeverShown() || clock.check(30000))
    return;

  PixelRect rc;
  PixelScalar x, y;
  canvas.select(Fonts::MapBold);
  PixelScalar offset_line1, offset_line2;

  PixelSize tsize1 = canvas.text_size(help_line1);
  PixelSize tsize2 = canvas.text_size(help_line2);
  PixelScalar width = max(tsize1.cx, tsize2.cx);

  if (tsize1.cx > tsize2.cx) {
    offset_line1 = 0;
    offset_line2 = (tsize1.cx - tsize2.cx) / 2;
  } else {
    offset_line2 = 0;
    offset_line1 = (tsize2.cx - tsize1.cx) / 2;
  }

  x = (rc_map.right - rc_map.left - width) / 2;
  y = (rc_map.bottom - rc_map.top) / 2 - tsize1.cy * 2 - tsize2.cy * 2;

  rc.left = x - Layout::FastScale(8);
  rc.right = x + width + Layout::FastScale(8);
  rc.top = y;
  rc.bottom = y + tsize1.cy + tsize2.cy + Layout::FastScale(4);

  canvas.black_pen();
  canvas.white_brush();
  canvas.round_rectangle(rc.left, rc.top,
                         rc.right, rc.bottom,
                         Layout::Scale(8), Layout::Scale(8));

  canvas.background_transparent();
  canvas.set_text_color(COLOR_BLACK);

  canvas.text(x + offset_line1, y + Layout::Scale(2), help_line1);
  canvas.text(x + offset_line2, y + tsize1.cy +  + Layout::Scale(2), help_line2);
  canvas.background_opaque();
}


