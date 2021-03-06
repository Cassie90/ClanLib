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
**    Harry Storbacka
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/Components/ribbon_menu.h"
#include "API/Display/Window/input_event.h"
#include "ribbon_menu_impl.h"

namespace clan
{

RibbonMenu_Impl::RibbonMenu_Impl() : current_item(-1), running(false)
{

}

void RibbonMenu_Impl::start(const Point &pos)
{
	if (running)
		throw Exception("Ribbon menu loop already running!");
	running = true;
	component->get_owner_component()->capture_mouse(true);
	
	current_item = -1;
	int height = update_item_boxes();
	component->set_window_geometry(Rect(pos, Size(519, height)), true);
	component->set_visible(true, false);
}

void RibbonMenu_Impl::end()
{
	if (running)
	{
		component->set_visible(false);
		component->get_owner_component()->capture_mouse(false);
		running = false;
	}
}

int RibbonMenu_Impl::update_item_boxes()
{
	item_boxes.clear();
	Rect client_box = component->get_size();
	int x = 1;
	int y = 25;
	for (size_t i = 0; i < items.size(); i++)
	{
		if (items[i].separator)
		{
			Rect item_box(1, y, 1 + part_menu_item_separator.get_css_width(), y + part_menu_item_separator.get_css_height());
			y = item_box.bottom;
			item_boxes.push_back(item_box);
		}
		else
		{
			Rect item_box(x, y, x + part_menu_item.get_css_width(), y + part_menu_item.get_css_height());
			y = item_box.bottom;
			item_boxes.push_back(item_box);
		}
	}
	return y + 18;
}

void RibbonMenu_Impl::on_render(Canvas &canvas, const Rect &update_box)
{
	update_item_boxes();
	Rect client_box = component->get_size();

	for (size_t i = 0; i < items.size(); i++)
	{
		if (items[i].separator)
		{
			Rect item_box = item_boxes[i];
			part_menu_item_separator.render_box(canvas, item_box);
		}
		else
		{
			Rect item_box = item_boxes[i];
			Rect item_content_box = part_menu_item.get_content_box(item_box);
			item_content_box.left += 7;
			Point icon_pos(item_content_box.left + 16 - items[i].image.get_width() / 2, item_content_box.top + item_content_box.get_height() / 2 - items[i].image.get_height() / 2);
			item_content_box.left += 32 + 5;
			if (current_item == (int)i)
			{
				part_menu_item_selected.render_box(canvas, item_box);
				items[i].image.draw(canvas, icon_pos.x, icon_pos.y);
				part_menu_item_selected.render_text(canvas, items[i].text, item_content_box);
			}
			else
			{
				part_menu_item.render_box(canvas, item_box);
				items[i].image.draw(canvas, icon_pos.x, icon_pos.y);
				part_menu_item.render_text(canvas, items[i].text, item_content_box);
			}
		}
	}
}

void RibbonMenu_Impl::on_resized()
{
	component->request_repaint();
}

void RibbonMenu_Impl::on_filter_message(std::shared_ptr<GUIMessage> &message)
{
	if (running)
	{
		std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(message);
		if (input_msg)
		{
			on_filter_input_message(input_msg);
		}

		std::shared_ptr<GUIMessage_FocusChange> focus_change_msg = std::dynamic_pointer_cast<GUIMessage_FocusChange>(message);
		if (focus_change_msg)
		{
			// Check we are not losing focus (FIXME: Required for X11 compatibility - this line is not required for win32)
			if ( ( focus_change_msg->focus_type == GUIMessage_FocusChange::losing_focus) && (message->target == component->get_owner_component()) )
			{
				end();
				message->consumed = true;
			}
		}
	}
}

void RibbonMenu_Impl::on_filter_input_message(std::shared_ptr<GUIMessage_Input> &message)
{
	message->input_event.mouse_pos = message->target->component_to_screen_coords(message->input_event.mouse_pos);
	if (message->input_event.device.get_type() == InputDevice::pointer)
	{
		Point pos = component->screen_to_component_coords(message->input_event.mouse_pos);
		Rect client_box = component->get_size();
		if (client_box.contains(pos))
		{
			int hot_item = -1;
			for (size_t i = 0; i < item_boxes.size(); i++)
			{
				if (item_boxes[i].contains(pos))
				{
					hot_item = i;
					break;
				}
			}
			if (hot_item != current_item)
			{
				current_item = hot_item;
				component->request_repaint();
			}

			if (message->input_event.type == InputEvent::released && current_item != -1)
			{
				end();
				if (!items[current_item].func_clicked.is_null())
					items[current_item].func_clicked.invoke();
			}

			message->consumed = true;
		}
		else
		{
			if (message->input_event.type == InputEvent::pressed)
			{
				end();
				message->consumed = true;
			}
			else
				message->consumed = true;
		}
	}
	else
	{
		// To do: fix this
		// on_keyboard_input(e);
		message->consumed = true;
	}
}

GUITopLevelDescription RibbonMenu_Impl::create_toplevel_description()
{
	GUITopLevelDescription window_desc;
	window_desc.set_allow_resize(false);
	window_desc.set_topmost(true);
	window_desc.show_caption(false);
	window_desc.set_tool_window(true);
	window_desc.set_visible(false);
	window_desc.set_drop_shadow(true);
	window_desc.set_using_gui_window_cache(true);
	window_desc.set_position(Rect(0,0,32,32), false);
	return window_desc;
}

}
