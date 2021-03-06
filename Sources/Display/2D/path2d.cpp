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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/Math/line_math.h"
#include "API/Core/Math/line_segment.h"
#include "API/Core/Math/bezier_curve.h"
#include "API/Display/2D/path2d.h"
#include "path2d_impl.h"
#include <cfloat>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Path2D Construction:

Path2D::Path2D() : impl(new Path2D_Impl())
{
}

Path2D::~Path2D()
{
}

/////////////////////////////////////////////////////////////////////////////
// Path2D Attributes:

bool Path2D::is_hole(PolygonOrientation orientation) const
{
	return impl->is_hole(orientation);
}

bool Path2D::is_inside_contour(const Path2D &other) const
{
	return impl->is_inside_contour(other);
}

bool Path2D::is_point_inside(const Pointf &P) const
{
	return impl->is_point_inside(P);
}

const std::vector<Pointf> &Path2D::get_contour_points() const
{
	return impl->get_contour_points();
}


/////////////////////////////////////////////////////////////////////////////
// Path2D Operations:

void Path2D::add_curve(BezierCurve &curve)
{
	impl->add_curve(curve);
}

void Path2D::add_line_to(const Pointf &point )
{
	impl->add_line_to(point);
}

void Path2D::add_line_to( const std::vector<Pointf> &points)
{
	impl->add_line_to(points);
}

void Path2D::reverse()
{
	impl->reverse();
}

/////////////////////////////////////////////////////////////////////////////
// Path2D Implementation:

}
