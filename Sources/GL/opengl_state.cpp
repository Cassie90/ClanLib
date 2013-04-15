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
**    Magnus Norddahl
**    Mark Page
*/

#include "GL/precomp.h"
#include "opengl_state.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "GL1/gl1_graphic_context_provider.h"

namespace clan
{

class BlendStateChangeset
{
public:
	BlendStateChangeset(BlendStateProvider_GL *from, BlendStateProvider_GL *to);
	void apply();
	void detach();

private:
	BlendStateProvider_GL *from;
	BlendStateProvider_GL *to;
	std::vector<std::shared_ptr<BlendStateChange> > changes;
};

class BlendStateChange_BlendFunc : public BlendStateChange
{
public:
	BlendStateChange_BlendFunc(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha)
	: src(src), dest(dest), src_alpha(src_alpha), dest_alpha(dest_alpha)
	{
	}

	void apply()
	{
		if( src == src_alpha && dest == dest_alpha )
		{
			if (glBlendFunc)
				glBlendFunc(OpenGL::to_enum(src), OpenGL::to_enum(dest));
		}
		else
		{
			if (glBlendFuncSeparate)
			{
				glBlendFuncSeparate( OpenGL::to_enum(src), OpenGL::to_enum(dest), OpenGL::to_enum(src_alpha), OpenGL::to_enum(dest_alpha) );
			}
			else
			{
				if (glBlendFunc)
					glBlendFunc(OpenGL::to_enum(src), OpenGL::to_enum(dest));
			}
		}
	}
private:
	BlendFunc src, dest, src_alpha, dest_alpha;
};

BlendStateChangeset::BlendStateChangeset(BlendStateProvider_GL *from, BlendStateProvider_GL *to)
: from(from), to(to)
{
	BlendFunc a_src, a_dest, a_src_alpha, a_dest_alpha;
	BlendFunc b_src, b_dest, b_src_alpha, b_dest_alpha;
	from->desc.get_blend_function(a_src, a_dest, a_src_alpha, a_dest_alpha);
	to->desc.get_blend_function(b_src, b_dest, b_src_alpha, b_dest_alpha);

	if ( (a_src != b_src) || (a_dest != b_dest) || (a_src_alpha != b_src_alpha) || (a_dest_alpha != b_dest_alpha) )
	{
		to->desc.set_blend_function(a_src, a_dest, a_src_alpha, a_dest_alpha);
		changes.push_back(std::shared_ptr<BlendStateChange>(new BlendStateChange_BlendFunc(a_src, a_dest, a_src_alpha, a_dest_alpha)));
	}
}
	
void BlendStateChangeset::apply()
{
	for (auto it = changes.begin(); it != changes.end(); ++it)
		(*it)->apply();
}

void BlendStateChangeset::detach()
{
	from->changesets.erase(from->changesets.find(to));
	to->changesets.erase(to->changesets.find(from));
}

BlendStateProvider_GL::BlendStateProvider_GL(const BlendStateDescription &desc) : desc(desc.clone())
{
}

BlendStateProvider_GL::~BlendStateProvider_GL()
{
	while (!changesets.empty())
		changesets.begin()->second->detach();
}

void BlendStateProvider_GL::apply(BlendStateProvider_GL *from, BlendStateProvider_GL *to)
{
	auto it = from->changesets.find(to);
	if (it != from->changesets.end())
	{
		it->second->apply();
	}
	else
	{
		std::shared_ptr<BlendStateChangeset> changeset(new BlendStateChangeset(from, to));
			
		from->changesets[to] = changeset;
		to->changesets[from] = changeset;
			
		changeset->apply();
	}
}

}
