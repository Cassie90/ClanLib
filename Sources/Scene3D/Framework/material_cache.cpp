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

#include "Scene3D/precomp.h"
#include "material_cache.h"
#include "API/Scene3D/Resources/scene_cache.h"
#include "Scene3D/scene_impl.h"

namespace clan
{

MaterialCache::MaterialCache(Scene_Impl *scene)
: scene(scene)
{
}

void MaterialCache::update(GraphicContext &gc, float time_elapsed)
{
	SceneCache::get(scene->get_resources()).update_textures(gc, time_elapsed);
}

Resource<Texture> MaterialCache::get_texture(GraphicContext &gc, const std::string &material_name, bool linear)
{
	return SceneCache::get(scene->get_resources()).get_texture(gc, material_name, linear);
}

}
