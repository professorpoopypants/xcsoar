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

#include "InfoBoxes/Content/Altitude.hpp"
#include "InfoBoxes/Panel/AltitudeInfo.hpp"
#include "InfoBoxes/Panel/AltitudeSimulator.hpp"
#include "InfoBoxes/InfoBoxWindow.hpp"
#include "InfoBoxes/InfoBoxManager.hpp"
#include "Units/Units.hpp"
#include "Interface.hpp"
#include "Components.hpp"
#include "Engine/Waypoint/Waypoint.hpp"
#include "Engine/Waypoint/Waypoints.hpp"
#include "Language/Language.hpp"
#include "Blackboard/DeviceBlackboard.hpp"
#include "Components.hpp"
#include "Simulator.hpp"
#include "Protection.hpp"

#include "Util/Macros.hpp"

#include <tchar.h>
#include <stdio.h>

/*
 * Subpart callback function pointers
 */

static gcc_constexpr_data
InfoBoxContentAltitude::PanelContent Panels[] = {
  InfoBoxContentAltitude::PanelContent (
    N_("Simulator"),
    LoadAltitudeSimulatorPanel),

  InfoBoxContentAltitude::PanelContent (
    N_("Info"),
    LoadAltitudeInfoPanel),
};

static gcc_constexpr_data
InfoBoxContentAltitude::DialogContent dlgContent = {
  ARRAY_SIZE(Panels), &Panels[0], false,
};

const InfoBoxContentAltitude::DialogContent *
InfoBoxContentAltitude::GetDialogContent() {
  return &dlgContent;
}

void
InfoBoxContentAltitudeNav::Update(InfoBoxData &data)
{
  const MoreData &basic = CommonInterface::Basic();

  if (!basic.NavAltitudeAvailable()) {
    data.SetInvalid();

    if (basic.pressure_altitude_available)
      data.SetComment(_("no QNH"));

    return;
  }

  const ComputerSettings &settings_computer = CommonInterface::GetComputerSettings();

  if (basic.baro_altitude_available &&
      settings_computer.features.nav_baro_altitude_enabled)
    data.SetTitle(InfoBoxFactory::MetaData[InfoBoxFactory::e_H_Baro].caption);
  else
    data.SetTitle(InfoBoxFactory::MetaData[InfoBoxFactory::e_HeightGPS].caption);

  data.SetValueFromAltitude(basic.nav_altitude);
  data.SetCommentFromAlternateAltitude(basic.nav_altitude);
}

void
InfoBoxContentAltitudeGPS::Update(InfoBoxData &data)
{
  const NMEAInfo &basic = CommonInterface::Basic();

  if (!basic.gps_altitude_available) {
    data.SetInvalid();
    return;
  }

  data.SetValueFromAltitude(basic.gps_altitude);
  data.SetCommentFromAlternateAltitude(basic.gps_altitude);
}

void
InfoBoxContentAltitudeAGL::Update(InfoBoxData &data)
{
  const DerivedInfo &calculated = CommonInterface::Calculated();

  if (!calculated.altitude_agl_valid) {
    data.SetInvalid();
    return;
  }

  data.SetValueFromAltitude(calculated.altitude_agl);
  data.SetCommentFromAlternateAltitude(calculated.altitude_agl);

  // Set Color (red/black)
  data.SetValueColor(calculated.altitude_agl <
      XCSoarInterface::GetComputerSettings().task.route_planner.safety_height_terrain ? 1 : 0);
}

void
InfoBoxContentAltitudeBaro::Update(InfoBoxData &data)
{
  const NMEAInfo &basic = CommonInterface::Basic();

  if (!basic.baro_altitude_available) {
    data.SetInvalid();

    if (basic.pressure_altitude_available)
      data.SetComment(_("no QNH"));

    return;
  }

  data.SetValueFromAltitude(basic.baro_altitude);
  data.SetCommentFromAlternateAltitude(basic.baro_altitude);
}

void
InfoBoxContentAltitudeQFE::Update(InfoBoxData &data)
{
  const NMEAInfo &basic = CommonInterface::Basic();

  if (!basic.gps_altitude_available) {
    data.SetInvalid();
    return;
  }

  fixed Value = basic.gps_altitude;

  const Waypoint *home_waypoint = way_points.GetHome();
  if (home_waypoint)
    Value -= home_waypoint->elevation;

  data.SetValueFromAltitude(Value);
  data.SetCommentFromAlternateAltitude(Value);
}

void
InfoBoxContentFlightLevel::Update(InfoBoxData &data)
{
  const NMEAInfo &basic = CommonInterface::Basic();
  const ComputerSettings &settings_computer =
    CommonInterface::GetComputerSettings();

  if (basic.pressure_altitude_available) {
    fixed Altitude = Units::ToUserUnit(basic.pressure_altitude, Unit::FEET);

    // Title color black
    data.SetTitleColor(0);

    // Set Value
    data.UnsafeFormatValue(_T("%03d"), iround(Altitude / 100));

    // Set Comment
    data.UnsafeFormatComment(_T("%dft"), iround(Altitude));

  } else if (basic.gps_altitude_available &&
             settings_computer.pressure_available) {
    // Take gps altitude as baro altitude. This is inaccurate but still fits our needs.
    const AtmosphericPressure &qnh = settings_computer.pressure;
    fixed Altitude = Units::ToUserUnit(qnh.QNHAltitudeToPressureAltitude(basic.gps_altitude), Unit::FEET);

    // Title color red
    data.SetTitleColor(1);

    // Set Value
    data.UnsafeFormatValue(_T("%03d"), iround(Altitude / 100));

    // Set Comment
    data.UnsafeFormatComment(_T("%dft"), iround(Altitude));

  } else if ((basic.baro_altitude_available || basic.gps_altitude_available) &&
             !settings_computer.pressure_available) {
    data.SetInvalid();
    data.SetComment(_("no QNH"));
  } else {
    data.SetInvalid();
  }
}
