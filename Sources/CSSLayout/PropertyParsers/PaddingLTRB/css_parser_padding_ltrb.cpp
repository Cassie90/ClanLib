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
#include "css_parser_padding_ltrb.h"
#include "API/CSSLayout/PropertyValues/css_value_padding_width.h"

namespace clan
{

std::vector<std::string> CSSParserPaddingLTRB::get_names()
{
	std::vector<std::string> names;
	names.push_back("padding-left");
	names.push_back("padding-top");
	names.push_back("padding-right");
	names.push_back("padding-bottom");
	return names;
}

void CSSParserPaddingLTRB::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValuePaddingWidth> width(new CSSValuePaddingWidth);
	if (equals(name, "padding-top"))
		width->value_type = CSSValuePaddingWidth::top_value;
	else if (equals(name, "padding-right"))
		width->value_type = CSSValuePaddingWidth::right_value;
	else if (equals(name, "padding-bottom"))
		width->value_type = CSSValuePaddingWidth::bottom_value;
	else if (equals(name, "padding-left"))
		width->value_type = CSSValuePaddingWidth::left_value;

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "inherit"))
			width->type = CSSValuePaddingWidth::type_inherit;
		else
			return;
	}
	else if (is_length(token) && pos == tokens.size())
	{
		CSSLength length;
		if (parse_length(token, length))
		{
			width->type = CSSValuePaddingWidth::type_length;
			width->length = length;
		}
		else
		{
			return;
		}
	}
	else if (token.type == CSSToken::type_percentage && pos == tokens.size())
	{
		width->type = CSSValuePaddingWidth::type_percentage;
		width->percentage = StringHelp::text_to_float(token.value);
	}
	else if (token.type == CSSToken::type_delim && token.value == "-")
	{
		token = next_token(pos, tokens);
		if (is_length(token) && pos == tokens.size())
		{
			CSSLength length;
			if (parse_length(token, length))
			{
				length.value = -length.value;
				width->type = CSSValuePaddingWidth::type_length;
				width->length = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == CSSToken::type_percentage && pos == tokens.size())
		{
			width->type = CSSValuePaddingWidth::type_percentage;
			width->percentage = -StringHelp::text_to_float(token.value);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(width));
}

}
