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
**    Harry Storbacka
*/


#pragma once

#include <memory>
#include "gui_layout.h"

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class GUIComponent;
class GUILayoutProvider_Corners;

/// \brief Component Anchor Point
enum ComponentAnchorPoint
{
	anchor_top_left,
	anchor_top_right,
	anchor_bottom_left,
	anchor_bottom_right,
	anchor_relative
};

/// \brief GUI base component class.
class CL_API_GUI GUILayoutCorners : public GUILayout
{
/// \name Construction
/// \{

public:
	GUILayoutCorners();

	virtual ~GUILayoutCorners();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Retrieves the provider.
	GUILayoutProvider_Corners *get_provider() const;

/// \}
/// \name Operations
/// \{

public:
	void add_component(
		GUIComponent *component,
		ComponentAnchorPoint ap_tl, int dist_tl_x, int dist_tl_y,
		ComponentAnchorPoint ap_br, int dist_br_x, int dist_br_y);

	/// \brief Remove component
	///
	/// \param component = GUIComponent
	void remove_component(GUIComponent *component);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}

/// \}
