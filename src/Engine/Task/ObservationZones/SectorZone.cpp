/* Copyright_License {

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

#include "SectorZone.hpp"
#include "Navigation/Geometry/GeoVector.hpp"

GeoPoint
SectorZone::GetBoundaryParametric(fixed t) const
{
  const Angle sweep = (end_radial - start_radial).AsBearing();
  const fixed l = GetRadius();
  const fixed c1 = sweep.Radians() * GetRadius();
  const fixed tt = t * (c1 + 2 * l);
  Angle a;
  fixed d;
  if (tt < l) {
    d = (tt / l) * GetRadius();
    a = start_radial;
  } else if (tt < l + c1) {
    d = GetRadius();
    a = start_radial + Angle::Radians(((tt - l) / c1) * sweep.Radians());
  } else {
    d = GetRadius() - (tt - l - c1) / l * GetRadius();
    a = end_radial;
  }
  return GeoVector(d, a).EndPoint(GetReference());
}


ObservationZone::Boundary
SectorZone::GetBoundary() const
{
  Boundary boundary;

  boundary.push_front(GetReference());
  boundary.push_front(GetSectorStart());
  boundary.push_front(GetSectorEnd());

  const unsigned steps = 20;
  const Angle delta = Angle::FullCircle() / steps;
  const Angle start = GetStartRadial().AsBearing();
  Angle end = GetEndRadial().AsBearing();
  if (end <= start + Angle::FullCircle() / 512)
    end += Angle::FullCircle();

  GeoVector vector(GetRadius(), start + delta);
  for (; vector.bearing < end; vector.bearing += delta)
    boundary.push_front(vector.EndPoint(GetReference()));

  return boundary;
}

fixed
SectorZone::ScoreAdjustment() const
{
  return fixed_zero;
}

void 
SectorZone::UpdateSector() 
{
  sector_start = GeoVector(GetRadius(), start_radial).EndPoint(GetReference());
  sector_end = GeoVector(GetRadius(), end_radial).EndPoint(GetReference());
}

bool 
SectorZone::IsInSector(const AircraftState &ref) const
{
  GeoVector f(GetReference(), ref.location);

  return f.distance <= GetRadius() && IsAngleInSector(f.bearing);
}

void
SectorZone::SetStartRadial(const Angle x)
{
  start_radial = x;
  UpdateSector();
}

void
SectorZone::SetEndRadial(const Angle x)
{
  end_radial = x;
  UpdateSector();
}

bool
SectorZone::IsAngleInSector(const Angle b) const
{
  // Quit early if we have a full circle
  if ((end_radial - start_radial).AsBearing() <= Angle::FullCircle() / 512)
    return true;

  return b.Between(start_radial, end_radial);
}

bool
SectorZone::Equals(const ObservationZonePoint &other) const
{
  const SectorZone &z = (const SectorZone &)other;

  return CylinderZone::Equals(other) &&
    start_radial == z.GetStartRadial() &&
    end_radial == z.GetEndRadial();
}
