/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#pragma once

#include "dock.h"

namespace clan
{

class DockedFrame;

class TabDock : public Dock
{
public:
	TabDock(DockingGroup *parent, int id);

private:
	bool is_docked(DockableComponent *dockable);
	void dock(DockableComponent *dockable);
	void undock(DockableComponent *dockable);
	bool is_empty() const { return tabs.empty(); }
	void set_active(DockableComponent *dockable);
	void activate_tab(int tab_index);
	int find_tab_index(DockableComponent *dockable) const;
	void on_button_close_clicked();
	bool on_input_pressed(const InputEvent &input_event, DockableComponent *dockable);
	bool on_input_doubleclick(const InputEvent &input_event, DockableComponent *dockable);

	class TabButton : public GUIComponent
	{
	public:
		TabButton(GUIComponent *parent) : GUIComponent(parent, "tab-button"), icon(0), label(0)
		{
			//icon = new ImageView(this);
			label = new Label(this);
		}
		ImageView *icon;
		Label *label;
	};

	GUIComponent *header;
	Label *title_label;
	PushButton *button_close;
	GUIComponent *frame_area;
	std::vector<DockedFrame *> tabs;
	GUIComponent *tab_button_row;
	std::vector<TabButton *> tab_buttons;
	
	int active_tab_index;
};

}

