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

namespace clan
{
/// \addtogroup clanCSSLayout_Layout clanCSSLayout Layout
/// \{

class CSSReplacedComponent
{
public:
	CSSReplacedComponent() : intrinsic_width(0), intrinsic_height(0), intrinsic_ratio(0.0f), intrinsic_has_width(false), intrinsic_has_height(false), intrinsic_has_ratio(false) { }
	virtual ~CSSReplacedComponent() { }

	/// \brief Set geometry
	///
	/// \param geometry = Rect
	virtual void set_geometry(const Rect &geometry) = 0;

private:
	int intrinsic_width;
	int intrinsic_height;
	float intrinsic_ratio;
	bool intrinsic_has_width;
	bool intrinsic_has_height;
	bool intrinsic_has_ratio;

	friend class CSSLayoutObject;
	friend class CSSReplacedLayout;
};

/// \brief Span Component Binder (templated class)
///
template<typename T>
class CSSReplacedComponentBinder : public CSSReplacedComponent
{
public:

	/// \brief Constructs a CSSReplacedComponentBinder
	///
	/// \param component = T
	CSSReplacedComponentBinder(T *component)
	: component(component)
	{
	}

	/// \brief Set geometry
	///
	/// \param geometry = Rect
	void set_geometry(const Rect &geometry)
	{
		component->set_geometry(geometry);
	}

private:
	T *component;
};

/// \}
}
