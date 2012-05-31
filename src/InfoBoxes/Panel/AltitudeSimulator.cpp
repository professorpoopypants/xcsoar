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

#include "AltitudeSimulator.hpp"
#include "Base.hpp"
#include "Interface.hpp"
#include "Components.hpp"
#include "Blackboard/DeviceBlackboard.hpp"
#include "Units/Units.hpp"
#include "Simulator.hpp"
#include "Form/PanelWidget.hpp"
#include "Form/Button.hpp"
#include "Form/Panel.hpp"
#include "Form/Frame.hpp"
#include "UIGlobals.hpp"
#include "Screen/Fonts.hpp"
#include "Screen/Layout.hpp"
#include "Formatter/UserUnits.hpp"


enum ControlIndex {
  BigPlus,
  LittlePlus,
  AltitudeValue,
  BigMinus,
  LittleMinus,
};

class AltitudeSimulatorPanel : public BaseAccessPanel, NumberButtonLayout {
protected:

  /**
   * The four buttons and the frame use the layout of the rectangles
   * created by NumberButtonLayout class
   */
  WndButton *big_plus, *big_minus, *little_plus, *little_minus;
  WndFrame *altitude_value;

  /**
   * used to avoid blackboard delays in showing values
   */
  fixed altitude_fast;

public:
  AltitudeSimulatorPanel(int id)
    :BaseAccessPanel(id) {}

protected:
  void Refresh();
  virtual void Prepare(ContainerWindow &parent, const PixelRect &rc);
  virtual void Unprepare();

  /* methods from ActionListener */
  virtual void OnAction(int id);

  void ChangeAltitude(const fixed step);
};

void
AltitudeSimulatorPanel::OnAction(int id)
{
  assert(is_simulator());

  switch (id) {
  case BigPlus:
    ChangeAltitude(fixed(+100));
    break;
  case LittlePlus:
    ChangeAltitude(fixed(+10));
    break;
  case BigMinus:
    ChangeAltitude(fixed(-100));
    break;
  case LittleMinus:
    ChangeAltitude(fixed(-10));
    break;
  default:
    BaseAccessPanel::OnAction(id);
  }
}

void
AltitudeSimulatorPanel::ChangeAltitude(const fixed step)
{
  const NMEAInfo &basic = CommonInterface::Basic();
  altitude_fast += (fixed)Units::ToSysAltitude(step);
  device_blackboard->SetAltitude(basic.gps_altitude +
                                 (fixed)Units::ToSysAltitude(step));
  Refresh();
}

void
AltitudeSimulatorPanel::Refresh()
{
  StaticString<32> buffer;
  FormatUserAltitude(altitude_fast, buffer.buffer(), true);
  altitude_value->SetCaption(buffer.c_str());
}

void
AltitudeSimulatorPanel::Prepare(ContainerWindow &parent,
                                const PixelRect &rc)
{
  BaseAccessPanel::Prepare(parent, rc);
  NumberButtonLayout::Prepare(parent, content_rc);

  const DialogLook &look = UIGlobals::GetDialogLook();

  ButtonWindowStyle button_style;
  button_style.TabStop();
  button_style.multiline();
  big_plus = new WndButton(GetClientAreaWindow(), look, _T("+100"), big_plus_rc,
                           button_style, this, BigPlus);
  big_plus->SetFont(Fonts::infobox);

  little_plus = new WndButton(GetClientAreaWindow(), look, _T("+10"), little_plus_rc,
                              button_style, this, LittlePlus);
  little_plus->SetFont(Fonts::infobox);

  big_minus = new WndButton(GetClientAreaWindow(), look, _T("-100"), big_minus_rc,
                            button_style, this, BigMinus);
  big_minus->SetFont(Fonts::infobox);

  little_minus = new WndButton(GetClientAreaWindow(), look, _T("-10"), little_minus_rc,
                               button_style, this, LittleMinus);
  little_minus->SetFont(Fonts::infobox);

  WindowStyle style_frame;
  altitude_value = new WndFrame(GetClientAreaWindow(), look,
                                value_rc.left, value_rc.top,
                                value_rc.right - value_rc.left,
                                value_rc.bottom - value_rc.top,
                                style_frame);
  altitude_value->SetVAlignCenter();
  altitude_value->SetAlignCenter();
  altitude_value->SetFont(Fonts::infobox);

  altitude_fast = CommonInterface::Basic().gps_altitude;
  Refresh();
}

void
AltitudeSimulatorPanel::Unprepare()
{
  delete big_plus;
  delete big_minus;
  delete little_plus;
  delete little_minus;
  delete altitude_value;
  BaseAccessPanel::Unprepare();
}

Widget *
LoadAltitudeSimulatorPanel(unsigned id)
{
  const NMEAInfo &basic = CommonInterface::Basic();

  if (!basic.gps.simulator)
    return NULL;

  return new AltitudeSimulatorPanel(id);
}
