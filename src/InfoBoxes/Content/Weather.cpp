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

#include "InfoBoxes/Content/Weather.hpp"
#include "InfoBoxes/Panel/Panel.hpp"
#include "InfoBoxes/Panel/WindSetupInfoBox.hpp"
#include "InfoBoxes/Data.hpp"
#include "Interface.hpp"
#include "Util/Macros.hpp"
#include "Units/Units.hpp"
#include "Language/Language.hpp"
#include "Formatter/UserUnits.hpp"
#include "Formatter/AngleFormatter.hpp"
#include "Screen/Layout.hpp"
#include "Renderer/WindArrowRenderer.hpp"
#include "UIGlobals.hpp"
#include "Look/Look.hpp"

#include <tchar.h>

void
UpdateInfoBoxHumidity(InfoBoxData &data)
{
  const NMEAInfo &basic = CommonInterface::Basic();
  if (!basic.humidity_available) {
    data.SetInvalid();
    return;
  }

  // Set Value
  data.UnsafeFormatValue( _T("%d"), (int)basic.humidity);
}

void
UpdateInfoBoxTemperature(InfoBoxData &data)
{
  const NMEAInfo &basic = CommonInterface::Basic();
  if (!basic.temperature_available) {
    data.SetInvalid();
    return;
  }

  // Set Value
  data.SetValue(_T("%2.1f"),
                    Units::ToUserTemperature(basic.temperature));

  data.SetValueUnit(Units::current.temperature_unit);
}

void
InfoBoxContentTemperatureForecast::Update(InfoBoxData &data)
{
  fixed temperature = CommonInterface::GetComputerSettings().forecast_temperature;
  data.SetValue(_T("%2.1f"),
                    Units::ToUserTemperature(temperature));

  data.SetValueUnit(Units::current.temperature_unit);
}

bool
InfoBoxContentTemperatureForecast::HandleKey(const InfoBoxKeyCodes keycode)
{
  switch(keycode) {
  case ibkUp:
    CommonInterface::SetComputerSettings().forecast_temperature += fixed(0.5);
    return true;

  case ibkDown:
    CommonInterface::SetComputerSettings().forecast_temperature -= fixed(0.5);
    return true;

  default:
    break;
  }

  return false;
}

/*
 * Subpart callback function pointers
 */

#ifdef __clang__
/* gcc gives "redeclaration differs in 'constexpr'" */
constexpr
#endif
const InfoBoxPanel wind_infobox_panels[] = {
  { N_("Setup"), LoadWindSetupInfoBoxPanel },
  { nullptr, nullptr }
};

const InfoBoxPanel *
InfoBoxContentWind::GetDialogContent()
{
  return wind_infobox_panels;
}

void
InfoBoxContentWindSpeed::Update(InfoBoxData &data)
{
  const DerivedInfo &info = CommonInterface::Calculated();
  if (!info.wind_available) {
    data.SetInvalid();
    return;
  }

  // Set Value
  data.SetValue(_T("%2.0f"),
                    Units::ToUserWindSpeed(info.wind.norm));

  // Set Unit
  data.SetValueUnit(Units::current.wind_speed_unit);

  // Set Comment
  data.SetComment(info.wind.bearing);
}

void
InfoBoxContentWindBearing::Update(InfoBoxData &data)
{
  const DerivedInfo &info = CommonInterface::Calculated();
  if (!info.wind_available) {
    data.SetInvalid();
    return;
  }

  data.SetValue(info.wind.bearing);

  TCHAR buffer[16];
  FormatUserWindSpeed(info.wind.norm, buffer, true, false);
  data.SetComment(buffer);
}

void
InfoBoxContentHeadWind::Update(InfoBoxData &data)
{
  const DerivedInfo &info = CommonInterface::Calculated();
  if (!info.head_wind_available) {
    data.SetInvalid();
    return;
  }

  // Set Value
  data.SetValue(_T("%2.0f"),
                    Units::ToUserWindSpeed(info.head_wind));

  // Set Unit
  data.SetValueUnit(Units::current.wind_speed_unit);
}

void
InfoBoxContentHeadWindSimplified::Update(InfoBoxData &data)
{
  const NMEAInfo &basic = CommonInterface::Basic();
  if (!basic.ground_speed_available || !basic.airspeed_available) {
    data.SetInvalid();
    return;
  }

  fixed value = basic.true_airspeed - basic.ground_speed;

  // Set Value
  data.SetValue(_T("%2.0f"), Units::ToUserWindSpeed(value));

  // Set Unit
  data.SetValueUnit(Units::current.wind_speed_unit);
}

void
InfoBoxContentWindArrow::Update(InfoBoxData &data)
{
  const DerivedInfo &info = CommonInterface::Calculated();
  if (!info.wind_available || info.wind.IsZero()) {
    data.SetInvalid();
    return;
  }

  data.SetCustom();

  TCHAR speed_buffer[16], bearing_buffer[16];
  FormatUserWindSpeed(info.wind.norm, speed_buffer, true, false);
  FormatBearing(bearing_buffer, ARRAY_SIZE(bearing_buffer), info.wind.bearing);

  StaticString<32> buffer;
  buffer.Format(_T("%s / %s"), bearing_buffer, speed_buffer);
  data.SetComment(buffer);
}

void
InfoBoxContentWindArrow::OnCustomPaint(Canvas &canvas, const PixelRect &rc)
{
  const auto &info = CommonInterface::Calculated();

  const RasterPoint pt = rc.GetCenter();

  UPixelScalar padding = Layout::FastScale(10);
  UPixelScalar size = std::min(rc.right - rc.left, rc.bottom - rc.top);

  if (size > padding)
    size -= padding;

  // Normalize the size because the Layout::Scale is applied
  // by the DrawArrow() function again
  size = size * 100 / Layout::Scale(100);

  auto angle = info.wind.bearing - CommonInterface::Basic().attitude.heading;

  PixelScalar length =
      std::min(size, (UPixelScalar)std::max(10, iround(info.wind.norm * 4)));

  PixelScalar offset = -length / 2;

  auto style = CommonInterface::GetMapSettings().wind_arrow_style;

  WindArrowRenderer renderer(UIGlobals::GetLook().wind_arrow_info_box);
  renderer.DrawArrow(canvas, pt, angle, length, style, offset);
}
