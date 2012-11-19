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
**    Arkadiusz Kalinowski
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "../Box2D/Box2D.h"
#include "API/Physics/World/physic_world.h"
#include "API/Physics/World/physic_world_description.h"

namespace clan
{
	class BodyDescription;

class PhysicWorld_Impl
{
public:

//																						_______________________
//																						C O N S T R U C T O R S
/// \name Construction
/// \{
	PhysicWorld_Impl();

	virtual ~PhysicWorld_Impl() { return; }
//																						___________________
//																						O P E R A T I O N S
/// \name Operations
/// \{
	void create(const PhysicWorldDescription &description);
	void step();
	void step(float timestep, int velocity_iterations, int position_iterations);

	b2Body		*create_body(const b2BodyDef &description);
	b2Joint		*create_joint(const b2JointDef &description);

//																						_____________
//																						S I G N A L S
	Signal_v1<float> sig_world_step;
	Signal_v0 sig_world_destroyed;
//																						___________________
//																						A T T R I B U T E S
/// \}
/// \name Attributes
/// \{
public:
	b2World world;
	float physic_scale; // in pixels per 1 Box2D meter. Defaults at 100.
	float timestep;
	int velocity_iterations;
	int position_iterations;
	std::string name;
};

}