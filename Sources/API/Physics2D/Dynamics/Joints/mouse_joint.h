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
**    Arkadiusz Kalinowski
*/



#pragma once

#include "../../api_physics2d.h"
#include "joint.h"
#include <memory>

namespace clan
{
/// \addtogroup clanPhysics_Dynamics clanPhysics Dynamics
/// \{

	class MouseJointDescription;
	class MouseJoint_Impl;
	class PhysicsContext;
	class Pointf;

/// \brief MouseJoint class.
class CL_API_PHYSICS MouseJoint : public Joint
{

/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	MouseJoint();

	/// \brief Constructs a MouseJoint object.
	///
	/// \param description = a MouseJoint Description object.
	MouseJoint(PhysicsContext &pc, const MouseJointDescription &description);

	virtual ~MouseJoint();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Short-cut function to determine if either body is inactive. 
	bool is_active() const;

/// \}
/// \name Operations
/// \{
public:
	MouseJoint &operator =(const MouseJoint &copy);

	bool grab(const Vec2f &target) const;
	bool grab(const Pointf &target) const;
	bool grab(const float x, const float y) const;
	bool grab(const int x, const int y) const;

	void release();

	//Add us
	// 	Clone the concrete MouseJoint
	//virtual MouseJoint	*Clone () const = 0;

	//virtual int 	GetChildCount () const = 0;
	//virtual bool 	TestPoint (const Vec2f &transformation, const Vec2f &point) const = 0;
	
	//virtual bool 	RayCast (b2RayCastOutput *output, const b2RayCastInput &input, const b2Transform &transform, int32 childIndex) const = 0;
	//virtual void 	ComputeAABB (b2AABB *aabb, const b2Transform &xf, int32 childIndex) const = 0;
	//virtual void 	ComputeMass (b2MassData *massData, float32 density) const = 0;
	//Add us
	
};

}

/// \}