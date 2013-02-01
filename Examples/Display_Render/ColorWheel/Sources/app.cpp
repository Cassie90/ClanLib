/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
*/

#include "precomp.h"
#include "app.h"
#include "colorwheel.h"
#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("ColorWheel Example");
	win_desc.set_size(Size( 800, 600 ), false);

	clan::DisplayWindow window(win_desc);
	clan::Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	clan::Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	std::string theme;
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw clan::Exception("No themes found");

	clan::GUIWindowManagerTexture wm(window);
	clan::GUIManager gui(wm, theme);

	clan::Canvas canvas = window.get_gc();

	// Deleted automatically by the GUI
	new ColorWheel(canvas, gui, clan::Rect(32, 32, Size(512, 512)));


	unsigned int time_last = clan::System::get_time();

	while (!quit)
	{
		unsigned int time_now = clan::System::get_time();
		float time_diff = (float) (time_now - time_last);
		time_last = time_now;

		canvas.clear(clan::Colorf(0.0f,0.0f,0.0f));

		wm.process();
		wm.draw_windows(canvas);

		canvas.flip(1);

		clan::KeepAlive::process();
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	switch (key.id)
	{
		case clan::keycode_escape:
			quit = true;
			break;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

