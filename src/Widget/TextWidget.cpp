/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
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

#include "TextWidget.hpp"
#include "UIGlobals.hpp"
#include "Form/Frame.hpp"
#include "Screen/Layout.hpp"
#include "Screen/Font.hpp"
#include "Look/DialogLook.hpp"

void
TextWidget::SetText(const TCHAR *text)
{
  WndFrame &w = (WndFrame &)GetWindow();
  w.SetText(text);
}

void
TextWidget::SetColor(Color _color)
{
  WndFrame &w = (WndFrame &)GetWindow();
  w.SetCaptionColor(_color);
}

PixelSize
TextWidget::GetMinimumSize() const
{
    if (fixed_height > 0)
      return { PixelScalar(0), fixed_height };

  const Font &font = UIGlobals::GetDialogLook().text_font;
  const PixelScalar height = Layout::GetTextPadding() + num_rows_text *
      (Layout::GetTextPadding() + font.GetHeight());

  return { PixelScalar(0), height };
}

PixelSize
TextWidget::GetMaximumSize() const
{
  PixelSize size = GetMinimumSize();

  if (IsDefined()) {
    const WndFrame &w = (const WndFrame &)GetWindow();
    PixelScalar text_height = w.GetTextHeight() + Layout::Scale(4);
    if (text_height > size.cy)
      size.cy = text_height;
  }

  if (fixed_height > 0)
    size.cy = fixed_height;

  return size;
}

void
TextWidget::Prepare(ContainerWindow &parent, const PixelRect &rc)
{
  WindowStyle style;
  style.Hide();

  WndFrame *w = new WndFrame(parent, UIGlobals::GetDialogLook(), rc, style);
  SetWindow(w);
}

void
TextWidget::Unprepare()
{
  DeleteWindow();
}


