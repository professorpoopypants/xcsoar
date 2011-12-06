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

#ifndef XCSOAR_TASK_CLOSE_PANEL_HPP
#define XCSOAR_TASK_CLOSE_PANEL_HPP

#include "Form/XMLWidget.hpp"

class OrderedTask;
class WndOwnerDrawFrame;
class WndFrame;
class WndButton;

class TaskClosePanel : public XMLWidget {
  bool *task_modified;

  WndOwnerDrawFrame *wTaskView;
  WndFrame *wStatus;
  WndButton *cmdRevert, *cmdClose;

public:
  TaskClosePanel(bool *_task_modified)
    :task_modified(_task_modified) {}

  /**
   * refreshes the buttons status of the text per the buttons
   */
  void RefreshStatus();

  virtual void Prepare(ContainerWindow &parent, const PixelRect &rc);

  /**
   * Closes the TaskManager and saves any changes
   */
  virtual void ReClick();

  /**
   * Before showing, checks if the task has been modified.
   * If not modified, closes the Main TaskManager window immediately.
   * If the task has been modified, shows the Close panel with Revert options
   */
virtual void Show(const PixelRect &rc);
};

#endif
