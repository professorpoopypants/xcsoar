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

#include "AlternatesListDialog.hpp"
#include "TaskDialogs.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Waypoint/WaypointDialogs.hpp"
#include "Screen/Layout.hpp"
#include "Screen/Font.hpp"
#include "Look/DialogLook.hpp"
#include "Components.hpp"
#include "Task/ProtectedTaskManager.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "Look/MapLook.hpp"
#include "Renderer/WaypointListRenderer.hpp"
#include "Language/Language.hpp"


void
AlternatesListWidget::OnPaintItem(Canvas &canvas, const PixelRect rc,
                                  unsigned index)
{
  assert(index < alternates.size());

  const ComputerSettings &settings = CommonInterface::GetComputerSettings();
  const Waypoint &waypoint = alternates[index].waypoint;
  const GlideResult& solution = alternates[index].solution;

  WaypointListRenderer::Draw(canvas, rc, waypoint, solution.vector.distance,
                             solution.SelectAltitudeDifference(settings.task.glide),
                             UIGlobals::GetDialogLook(),
                             UIGlobals::GetMapLook().waypoint,
                             CommonInterface::GetMapSettings().waypoint);
}

void
AlternatesListWidget::Update()
{
  ProtectedTaskManager::Lease lease(*protected_task_manager);
  alternates = lease->GetAlternates();
}

void
AlternatesListWidget::CreateButtons(WidgetDialog &dialog)
{
  goto_button = dialog.AddButton(_("Goto"), *this, GOTO);
  details_button = dialog.AddButton(_("Details"), mrOK);
  cancel_button = dialog.AddButton(_("Close"), mrCancel);
}

void
AlternatesListWidget::Prepare(ContainerWindow &parent, const PixelRect &rc)
{
  UPixelScalar item_height = dialog_look.list.font_bold->GetHeight()
    + Layout::Scale(6) + dialog_look.small_font->GetHeight();
  assert(item_height > 0);

  CreateList(parent, dialog_look, rc, item_height);

  GetList().SetLength(alternates.size());
}

void
AlternatesListWidget::OnActivateItem(unsigned index)
{
  details_button->OnClicked();
}

void
AlternatesListWidget::OnAction(int id)
{
  switch (id) {
  case GOTO:
    unsigned index = GetCursorIndex();

    auto const &item = alternates[index];
    auto const &waypoint = item.waypoint;

    protected_task_manager->DoGoto(waypoint);
    cancel_button->OnClicked();

    break;
  }
}

void
dlgAlternatesListShowModal()
{
  if (protected_task_manager == nullptr)
    return;

  const DialogLook &dialog_look = UIGlobals::GetDialogLook();

  AlternatesListWidget widget(dialog_look);
  widget.Update();

  WidgetDialog dialog(dialog_look);
  dialog.CreateFull(UIGlobals::GetMainWindow(), _("Alternates"), &widget);
  widget.CreateButtons(dialog);

  int i = dialog.ShowModal() == mrOK
    ? (int)widget.GetCursorIndex()
    : -1;
  dialog.StealWidget();

  if (i < 0 || (unsigned)i >= widget.alternates.size())
    return;

  dlgWaypointDetailsShowModal(widget.alternates[i].waypoint);
}
