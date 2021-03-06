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

#include "CSSLayout/precomp.h"
#include "css_parser_border_radius_corner.h"
#include "API/CSSLayout/PropertyValues/css_value_border_radius.h"

namespace clan
{

std::vector<std::string> CSSParserBorderRadiusCorner::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-top-right-radius");
	names.push_back("border-bottom-right-radius");
	names.push_back("border-bottom-left-radius");
	names.push_back("border-top-left-radius");
	return names;
}

void CSSParserBorderRadiusCorner::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueBorderRadius> radius(new CSSValueBorderRadius());
	if (equals(name, "border-top-right-radius"))
		radius->value_type = CSSValueBorderRadius::top_right_value;
	else if (equals(name, "border-bottom-right-radius"))
		radius->value_type = CSSValueBorderRadius::bottom_right_value;
	else if (equals(name, "border-bottom-left-radius"))
		radius->value_type = CSSValueBorderRadius::bottom_left_value;
	else if (equals(name, "border-top-left-radius"))
		radius->value_type = CSSValueBorderRadius::top_left_value;

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && equals(token.value, "inherit") && pos == tokens.size())
	{
		radius->type = CSSValueBorderRadius::type_inherit;
		inout_values.push_back(std::move(radius));
		return;
	}
	else if (is_length(token))
	{
		if (!parse_length(token, radius->length1))
		{
			debug_parse_error(name, tokens);
			return;
		}
		radius->type = CSSValueBorderRadius::type_one_value;
		radius->value_type1 = CSSValueBorderRadius::value_type_length;
	}
	else if (token.type == CSSToken::type_percentage)
	{
		radius->type = CSSValueBorderRadius::type_one_value;
		radius->value_type1 = CSSValueBorderRadius::value_type_percentage;
		radius->percentage1 = StringHelp::text_to_float(token.value);
	}
	else
	{
		debug_parse_error(name, tokens);
		return;
	}

	if (pos != tokens.size())
	{
		token = next_token(pos, tokens);
		if (is_length(token) && pos == tokens.size() && parse_length(token, radius->length2))
		{
			radius->type = CSSValueBorderRadius::type_two_values;
			inout_values.push_back(std::move(radius));
		}
		else if (token.type == CSSToken::type_percentage)
		{
			radius->type = CSSValueBorderRadius::type_two_values;
			radius->value_type2 = CSSValueBorderRadius::value_type_percentage;
			radius->percentage2 = StringHelp::text_to_float(token.value);
			inout_values.push_back(std::move(radius));
		}
		else
		{
			debug_parse_error(name, tokens);
			return;
		}
	}
	else
	{
		inout_values.push_back(std::move(radius));
	}
}

}
