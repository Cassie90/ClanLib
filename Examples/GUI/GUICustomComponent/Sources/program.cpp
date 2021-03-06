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
**    Mark Page
*/

#include "precomp.h"
#include "program.h"
#include "app.h"

int Program::main(const std::vector<std::string> &args)
{
	// Initialize ClanLib base components
	SetupCore setup_core;

	// Initialize the ClanLib display component
	SetupDisplay setup_display;

	// We support all display targets, in order listed here
	SetupD3D setup_d3d;
	SetupGL setup_gl;
	SetupSWRender setup_swrender;

	SetupGUI setup_gui;

	// Start the Application
	App app;
	int retval = app.main(args);
	return retval;
}

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);
