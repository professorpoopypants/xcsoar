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

#include "AltitudeInfo.hpp"
#include "Base.hpp"
#include "Interface.hpp"
#include "Components.hpp"
#include "Blackboard/DeviceBlackboard.hpp"
#include "Language/Language.hpp"
#include "Units/Units.hpp"
#include "Formatter/UserUnits.hpp"
#include "Simulator.hpp"
#include "Dialogs/dlgInfoBoxAccess.hpp"
#include "Form/Util.hpp"
#include "Form/DataField/String.hpp"
#include "Form/RowFormWidget.hpp"
#include "Form/Panel.hpp"
#include "Form/DataField/Listener.hpp"
#include "Form/DataField/Float.hpp"
#include "Blackboard/BlackboardListener.hpp"
#include "Operation/MessageOperationEnvironment.hpp"
#include "UIGlobals.hpp"
#include "Screen/Layout.hpp"
#include "Util/StaticString.hpp"

enum ControlIndex {
  Agl,
  Baro,
  Gps,
  Terrain,
  Spacer,
  Qnh,
  LastItemInList,
};

class AltitudeInfoPanel : public BaseAccessPanel, NullBlackboardListener,
  DataFieldListener {
protected:
  RowFormWidget *row_form_widget;

public:
  AltitudeInfoPanel(unsigned id)
    :BaseAccessPanel(id) {
    row_form_widget = new RowFormWidget(UIGlobals::GetDialogLook()) ;
  }

  ~AltitudeInfoPanel() {
    delete row_form_widget;
  }

  void Refresh();

  virtual void Initialise(ContainerWindow &parent, const PixelRect &rc);
  virtual void Prepare(ContainerWindow &parent, const PixelRect &rc);
  virtual void Show(const PixelRect &rc);
  virtual void Hide();

protected:
  virtual void OnGPSUpdate(const MoreData &basic);

  /* methods from DataFieldListener */
  virtual void OnModified(DataField &df);

  void OnQNH(DataFieldFloat& sender);
};

void
AltitudeInfoPanel::OnModified(DataField &df)
{
  if (row_form_widget->IsDataField(Qnh, df))
    OnQNH((DataFieldFloat&)df);
}

void
AltitudeInfoPanel::OnQNH(DataFieldFloat& sender)
{
  ComputerSettings &settings_computer =
    CommonInterface::SetComputerSettings();

  settings_computer.pressure = Units::FromUserPressure(sender.GetAsFixed());
  settings_computer.pressure_available.Update(CommonInterface::Basic().clock);

  MessageOperationEnvironment env;
  device_blackboard->SetQNH(Units::FromUserPressure(sender.GetAsFixed()), env);

  Refresh();
}

void
AltitudeInfoPanel::Refresh()
{
  const DerivedInfo &calculated = CommonInterface::Calculated();
  const NMEAInfo &basic = CommonInterface::Basic();
  StaticString<32> temp;

  if (!calculated.altitude_agl_valid) {
    row_form_widget->SetText(Agl, _("N/A"));
  } else {
    // Set Value
    FormatUserAltitude(calculated.altitude_agl, temp.buffer(), temp.length());
    row_form_widget->SetText(Agl, temp.c_str());
  }

  if (!basic.baro_altitude_available) {
    row_form_widget->SetText(Baro, _("N/A"));
  } else {
    // Set Value
    FormatUserAltitude(basic.baro_altitude, temp.buffer(), temp.length());
    row_form_widget->SetText(Baro, temp.c_str());
  }

  if (!basic.gps_altitude_available) {
    row_form_widget->SetText(Gps, _("N/A"));
  } else {
    // Set Value
    FormatUserAltitude(basic.gps_altitude, temp.buffer(), temp.length());
    row_form_widget->SetText(Gps, temp.c_str());
  }

  if (!calculated.terrain_valid){
    row_form_widget->SetText(Terrain, _("N/A"));
  } else {
    // Set Value
    FormatUserAltitude(calculated.terrain_altitude,
                              temp.buffer(), temp.length());
    row_form_widget->SetText(Terrain, temp.c_str());
  }
}
void
AltitudeInfoPanel::Initialise(ContainerWindow &parent, const PixelRect &rc)
{
  BaseAccessPanel::Initialise(parent, rc);
  row_form_widget->Initialise(GetClientAreaWindow(),
                              GetClientAreaWindow().GetClientRect());
}
void
AltitudeInfoPanel::Prepare(ContainerWindow &parent, const PixelRect &rc)
{
  BaseAccessPanel::Prepare(parent, rc);
  row_form_widget->Prepare(GetClientAreaWindow(), content_rc);

  const DerivedInfo &calculated = CommonInterface::Calculated();
  const NMEAInfo &basic = CommonInterface::Basic();
  StaticString<32> temp;

  if (!calculated.altitude_agl_valid) {
    row_form_widget->AddReadOnly(_T("H AGL"), NULL, _("N/A"));
  } else {
    FormatUserAltitude(calculated.altitude_agl, temp.buffer(), temp.length());
    row_form_widget->AddReadOnly(_T("H AGL"), NULL, temp.c_str());
  }

  if (!basic.baro_altitude_available) {
    row_form_widget->AddReadOnly(_T("H Baro"), NULL, _("N/A"));
  } else {
    FormatUserAltitude(basic.baro_altitude, temp.buffer(), temp.length());
    row_form_widget->AddReadOnly(_T("H Baro"), NULL, temp.c_str());
  }

  if (!basic.gps_altitude_available) {
    row_form_widget->AddReadOnly(_T("H GPS"), NULL, _("N/A"));
  } else {
    FormatUserAltitude(basic.gps_altitude, temp.buffer(), temp.length());
    row_form_widget->AddReadOnly(_T("H GPS"), NULL, temp.c_str());
  }

  if (!calculated.terrain_valid){
    row_form_widget->AddReadOnly(_T("H GND"), NULL, _("N/A"));
  } else {
    FormatUserAltitude(calculated.terrain_altitude,
                       temp.buffer(), temp.length());
    row_form_widget->AddReadOnly(_T("H GND"), NULL, temp.c_str());
  }

  row_form_widget->AddSpacer();

  row_form_widget->AddFloat(_("QNH"),
    _("Area pressure for barometric altimeter calibration.  This is set automatically if Vega connected."),
    GetUserPressureFormat(), GetUserPressureFormat(),
    Units::ToUserPressure(Units::ToSysUnit(fixed(850), Unit::HECTOPASCAL)),
    Units::ToUserPressure(Units::ToSysUnit(fixed(1300), Unit::HECTOPASCAL)),
    Units::ToUserPressure(Units::ToSysUnit(fixed_one, Unit::HECTOPASCAL)),
    false,
    UnitGroup::PRESSURE,
    CommonInterface::GetComputerSettings().pressure.GetHectoPascal(),
    this);
}

void
AltitudeInfoPanel::Show(const PixelRect &rc)
{
  unsigned controls = LastItemInList - 1;
  unsigned controls_height = Layout::GetMinimumControlHeight() * controls;

  unsigned controls_width = max((PixelScalar)(
                                (rc.right - rc.left) / 2),
                                (PixelScalar)100);
  PixelRect new_rect;
  new_rect.left = (rc.right - rc.left - controls_width) / 2;
  new_rect.top = (rc.bottom - rc.top - controls_height) / 2;
  new_rect.right = new_rect.left + controls_width;
  new_rect.bottom = rc.bottom;

  row_form_widget->Show(new_rect);
  CommonInterface::GetLiveBlackboard().AddListener(*this);

  BaseAccessPanel::Show(rc);
}

void
AltitudeInfoPanel::Hide()
{
  CommonInterface::GetLiveBlackboard().RemoveListener(*this);
  BaseAccessPanel::Hide();
  row_form_widget->Hide();
}

void
AltitudeInfoPanel::OnGPSUpdate(const MoreData &basic)
{
  Refresh();
}

Widget *
LoadAltitudeInfoPanel(unsigned id)
{
  return new AltitudeInfoPanel(id);
}
