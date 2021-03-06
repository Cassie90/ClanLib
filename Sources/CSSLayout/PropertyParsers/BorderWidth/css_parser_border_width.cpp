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
#include "css_parser_border_width.h"
#include "API/CSSLayout/PropertyValues/css_value_border_width.h"

namespace clan
{

std::vector<std::string> CSSParserBorderWidth::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-width");
	return names;
}

void CSSParserBorderWidth::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	CSSValueBorderWidth border_widths[4];
	int count;
	size_t pos = 0;
	for (count = 0; count < 4; count++)
	{
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident)
		{
			if (equals(token.value, "thin"))
			{
				border_widths[count].type = CSSValueBorderWidth::type_thin;
			}
			else if (equals(token.value, "medium"))
			{
				border_widths[count].type = CSSValueBorderWidth::type_medium;
			}
			else if (equals(token.value, "thick"))
			{
				border_widths[count].type = CSSValueBorderWidth::type_thick;
			}
			else if (equals(token.value, "inherit") && count == 0 && pos == tokens.size())
			{
				border_widths[0].type = CSSValueBorderWidth::type_inherit;

				inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::top_value, border_widths[0])));
				inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::right_value, border_widths[0])));
				inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::bottom_value, border_widths[0])));
				inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::left_value, border_widths[0])));
				return;
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else if (is_length(token))
		{
			CSSLength length;
			if (parse_length(token, length))
			{
				border_widths[count].type = CSSValueBorderWidth::type_length;
				border_widths[count].length = length;
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else if (token.type == CSSToken::type_null)
		{
			break;
		}
		else
		{
			debug_parse_error(name, tokens);
			return;
		}
	}

	if (pos == tokens.size())
	{
		switch (count)
		{
		case 1:
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::top_value, border_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::right_value, border_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::bottom_value, border_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::left_value, border_widths[0])));
			break;
		case 2:
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::top_value, border_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::right_value, border_widths[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::bottom_value, border_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::left_value, border_widths[1])));
			break;
		case 3:
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::top_value, border_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::right_value, border_widths[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::bottom_value, border_widths[2])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::left_value, border_widths[1])));
			break;
		case 4:
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::top_value, border_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::right_value, border_widths[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::bottom_value, border_widths[2])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::left_value, border_widths[3])));
			break;
		default:
			break;
		}
	}
}

}
