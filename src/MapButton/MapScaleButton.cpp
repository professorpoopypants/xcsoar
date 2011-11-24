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

#include "MapButton/MapScaleButton.hpp"
#include "Screen/Canvas.hpp"
#include "Screen/Graphics.hpp"
#include "Screen/Icon.hpp"
#include "Screen/Layout.hpp"
#include "Screen/Fonts.hpp"
#include "Screen/Color.hpp"
#include "Units/UnitsFormatter.hpp"
#include "Look/MapLook.hpp"
#include "InfoBoxes/InfoBoxManager.hpp"
#include "UIState.hpp"
#include "Interface.hpp"
#include "Terrain/RasterWeather.hpp"
#include "SettingsComputer.hpp"

#include "LogFile.hpp" //debug

MapScaleButton::MapScaleButton():
  MapButton(),
  map_width(fixed_zero),
  auto_zoom_enabled(false),
  replay(false),
  simulator(false)
{
}

static const SETTINGS_COMPUTER &
SettingsComputer()
{
  return CommonInterface::SettingsComputer();
}

void
MapScaleButton::Draw(Canvas &canvas, const PixelRect &rc_map)
{
  LogStartUp(_T("MapScaleButton::Draw()"));
  TCHAR buffer[80];

  canvas.select(Fonts::MapBold);
  Units::FormatUserMapScale(map_width, buffer,
                            sizeof(buffer) / sizeof(TCHAR), true);
  PixelSize text_size = canvas.text_size(buffer);

  UPixelScalar height = Fonts::MapBold.GetCapitalHeight() + Layout::Scale(2);
  // 2: add 1pix border

  canvas.fill_rectangle(size.left + Layout::Scale(4), size.bottom - height,
                        size.left + text_size.cx + Layout::Scale(11),
                        size.bottom, COLOR_WHITE);

  canvas.background_transparent();
  canvas.set_text_color(COLOR_BLACK);

  canvas.text(size.left + Layout::Scale(7),
              size.bottom - Fonts::MapBold.GetAscentHeight() - Layout::Scale(1),
              buffer);

  look->hBmpMapScaleLeft.Draw(canvas, size.left, size.bottom - height);
  look->hBmpMapScaleRight.Draw(canvas, size.left + Layout::Scale(9) +
                               text_size.cx,
                               size.bottom - height);

  buffer[0] = '\0';
  /*
  if (auto_zoom_enabled)
    _tcscat(buffer, _T("AUTO "));

  switch (follow_mode) {
  case FOLLOW_SELF:
    break;

  case FOLLOW_PAN:
    _tcscat(buffer, _T("PAN "));
    break;
  }
*/
  const UIState &ui_state = CommonInterface::GetUIState();
  if (ui_state.auxiliary_enabled) {
    _tcscat(buffer, InfoBoxManager::GetCurrentPanelName());
    _tcscat(buffer, _T(" "));
  }

  if (replay)
    _tcscat(buffer, _T("REPLAY "));
  else if (simulator) {
    _tcscat(buffer, _("Simulator"));
    _tcscat(buffer, _T(" "));
  }

  if (SettingsComputer().ballast_timer_active) {
    TCHAR TEMP[20];
    _stprintf(TEMP, _T("BALLAST %d LITERS "),
              (int)SettingsComputer().glide_polar_task.GetBallastLitres());
    _tcscat(buffer, TEMP);
  }

  if (weather != NULL && weather->GetParameter() > 0) {
    const TCHAR *label = weather->ItemLabel(weather->GetParameter());
    if (label != NULL)
      _tcscat(buffer, label);
  }

  if (buffer[0]) {
    int y = size.bottom - height;

    canvas.select(Fonts::Title);
    canvas.background_opaque();
    canvas.set_background_color(COLOR_WHITE);

    canvas.text(size.left, y - canvas.text_size(buffer).cy, buffer);
  }


  ///
  //size.right = size.left + bmp->GetSize().cx + Layout::Scale(6);

}

void
MapScaleButton::Update(fixed _map_width, const MapLook &_look,
                       RasterWeather* _weather,
                       bool _auto_zoom_enabled, bool _replay,
                       bool _simulator)
{
  map_width = _map_width;
  look = &_look;
  weather = _weather;
  auto_zoom_enabled = _auto_zoom_enabled;
  replay = _replay;
  simulator = _simulator;
}
