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

#include "Profile/RouteProfile.hpp"
#include "Profile/Profile.hpp"
#include "Engine/Route/Config.hpp"

void
Profile::Load(RoutePlannerConfig &settings)
{
  Get(ProfileKeys::SafetyAltitudeTerrain, settings.safety_height_terrain);
  GetEnum(ProfileKeys::RoutePlannerMode, settings.mode);
  Get(ProfileKeys::RoutePlannerAllowClimb, settings.allow_climb);
  Get(ProfileKeys::RoutePlannerUseCeiling, settings.use_ceiling);
  GetEnum(ProfileKeys::TurningReach, settings.reach_calc_mode);
  /** Hard code to Task MC.
   *  Leave architecture in place, but remove option from UI
   */
  settings.reach_polar_mode = RoutePlannerConfig::Polar::TASK;
}
