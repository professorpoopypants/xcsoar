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

#include "WindEdit.hpp"
#include "Base.hpp"
#include "Form/Edit.hpp"
#include "Form/DataField/Float.hpp"
#include "Form/DataField/Enum.hpp"
#include "Form/DataField/Boolean.hpp"
#include "Interface.hpp"
#include "Profile/ProfileKeys.hpp"
#include "Units/Units.hpp"
#include "Units/Group.hpp"
#include "Interface.hpp"
#include "Form/RowFormWidget.hpp"
#include "Form/DataField/Listener.hpp"
#include "UIGlobals.hpp"
#include "Language/Language.hpp"
#include "Screen/Layout.hpp"
#include "Look/DialogLook.hpp"
#include "Screen/Layout.hpp"

enum ControlIndex {
  AutoWind,
  Spacer,
  WindSpeed,
  WindDirection,
  Spacer2,
  TrailDrift,
  LastItemInList,
};

class WindEditPanel: public BaseAccessPanel, DataFieldListener {
protected:
  RowFormWidget *row_form_widget;

public:
  WindEditPanel(unsigned id)
    :BaseAccessPanel(id) {
    row_form_widget = new RowFormWidget(UIGlobals::GetDialogLook());
  }

  ~WindEditPanel() {
    delete row_form_widget;
  }

  virtual void Prepare(ContainerWindow &parent, const PixelRect &rc);
  virtual void Initialise(ContainerWindow &parent, const PixelRect &rc) {
    BaseAccessPanel::Initialise(parent, rc);
    row_form_widget->Initialise(GetClientAreaWindow(), rc);
  }
  virtual void Unprepare() {
    BaseAccessPanel::Unprepare();
    row_form_widget->Unprepare();
  }

  /**
   * Centers the RowFormWidget controls vertically on the screen
   * Also shrinks the vertical by 50% and centers horizontally
   */
  virtual void Show(const PixelRect &rc);

protected:
  /* methods from DataFieldListener */
  virtual void OnModified(DataField &df);

private:
  void OnWindSpeed(DataFieldFloat &Sender);
  void OnWindDirection(DataFieldFloat &Sender);
  void OnAutoWind(DataFieldEnum &Sender);
  void OnTrailDrift(DataFieldBoolean &Sender);
};

void
WindEditPanel::Show(const PixelRect &rc)
{
  unsigned controls = LastItemInList - 1;
  unsigned controls_height = Layout::GetMinimumControlHeight() * controls;

  unsigned controls_width = max((PixelScalar)((content_rc.right - content_rc.left) / 2),
                                (PixelScalar)100);
  PixelRect new_rect;
  new_rect.left = (content_rc.right - content_rc.left - controls_width) / 2;
  new_rect.top = (content_rc.bottom - content_rc.top - controls_height) / 2;
  new_rect.right = new_rect.left + controls_width;
  new_rect.bottom = content_rc.bottom;

  row_form_widget->Show(new_rect);
  BaseAccessPanel::Show(rc);
}

void
WindEditPanel::OnModified(DataField &df)
{
  if (row_form_widget->IsDataField(WindSpeed, df))
    OnWindSpeed((DataFieldFloat&)df);

  else if (row_form_widget->IsDataField(WindDirection, df))
    OnWindDirection((DataFieldFloat&)df);

  else if (row_form_widget->IsDataField(AutoWind, df))
    OnAutoWind((DataFieldEnum&)df);

  else if (row_form_widget->IsDataField(TrailDrift, df))
    OnTrailDrift((DataFieldBoolean&)df);
}

void
WindEditPanel::OnWindSpeed(DataFieldFloat &Sender)
{
  const NMEAInfo &basic = XCSoarInterface::Basic();
  WindSettings &settings = CommonInterface::SetComputerSettings().wind;
  const bool external_wind = basic.external_wind_available &&
    settings.use_external_wind;

  if (!external_wind) {
    settings.manual_wind.norm = Units::ToSysWindSpeed(Sender.GetAsFixed());
    settings.manual_wind_available.Update(basic.clock);
  }
}

void
WindEditPanel::OnWindDirection(DataFieldFloat &Sender)
{
  const NMEAInfo &basic = XCSoarInterface::Basic();
  WindSettings &settings = CommonInterface::SetComputerSettings().wind;
  const bool external_wind = basic.external_wind_available &&
    settings.use_external_wind;

  if (!external_wind) {
    settings.manual_wind.bearing = Angle::Degrees(Sender.GetAsFixed());
    settings.manual_wind_available.Update(basic.clock);
  }
}

void
WindEditPanel::OnAutoWind(DataFieldEnum &Sender)
{
  WindSettings &settings = CommonInterface::SetComputerSettings().wind;
  settings.SetLegacyAutoWindMode(Sender.GetAsInteger());
}

void
WindEditPanel::OnTrailDrift(DataFieldBoolean &Sender)
{
  XCSoarInterface::SetMapSettings().trail_drift_enabled = Sender.GetAsBoolean();
}

void
WindEditPanel::Prepare(ContainerWindow &parent, const PixelRect &rc)
{

  BaseAccessPanel::Prepare(parent, rc);

  const NMEAInfo &basic = XCSoarInterface::Basic();
  const WindSettings &settings = CommonInterface::GetComputerSettings().wind;
  const bool external_wind = basic.external_wind_available &&
    settings.use_external_wind;

  row_form_widget->Prepare(GetClientAreaWindow(), rc);

  if (external_wind) {
    static gcc_constexpr_data StaticEnumChoice external_wind_list[] = {
      { 0, N_("External") },
      { 0 }
    };
    row_form_widget->AddEnum(_("Auto. wind"),
      _("This allows switching on or off the automatic wind algorithm.  When the algorithm is switched off, the pilot is responsible for setting the wind estimate.&#10;[Circling] Requires only a GPS source.&#10;[ZigZag] Requires an intelligent vario with airspeed output.&#10;[Both] Use ZigZag and circling."),
      external_wind_list, settings.GetLegacyAutoWindMode(), this);
  } else {

    static gcc_constexpr_data StaticEnumChoice auto_wind_list[] = {
      { AUTOWIND_NONE, N_("Manual") },
      { AUTOWIND_CIRCLING, N_("Circling") },
      { AUTOWIND_ZIGZAG, N_("ZigZag") },
      { AUTOWIND_CIRCLING | AUTOWIND_ZIGZAG, N_("Both") },
      { 0 }
    };

    row_form_widget->AddEnum(_("Auto. wind"),
      _("This allows switching on or off the automatic wind algorithm.  When the algorithm is switched off, the pilot is responsible for setting the wind estimate.&#10;[Circling] Requires only a GPS source.&#10;[ZigZag] Requires an intelligent vario with airspeed output.&#10;[Both] Use ZigZag and circling."),
      auto_wind_list, settings.GetLegacyAutoWindMode(), this);
  }

  row_form_widget->AddSpacer();

  row_form_widget->AddFloat(_("Wind Speed"),
                            _("Manual adjustment of wind speed."), _T("%.0f"),
                            _T("%.0f"), fixed_zero, fixed(130), fixed(1),
                            false, UnitGroup::WIND_SPEED,
                            CommonInterface::Calculated().GetWindOrZero().norm,
                            this);

  row_form_widget->AddFloat(_("Direction"),
                            _("Manual adjustment of wind direction."),
                            _T("%.0f°"), _T("%.0f"), fixed_zero, fixed(355),
                            fixed(5), false,
                            CommonInterface::Calculated().GetWindOrZero().
                            bearing.Degrees(), this);

  row_form_widget->AddSpacer();

  row_form_widget->AddBoolean(_("Trail drift"),
    _("Determines whether the snail trail is drifted with the wind when displayed in circling mode."),
    XCSoarInterface::GetMapSettings().trail_drift_enabled, this);
}

Widget *
LoadWindEditPanel(unsigned id)
{
  return new WindEditPanel(id);
}
