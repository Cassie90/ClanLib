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
#include "css_parser_border_image.h"
#include "API/CSSLayout/PropertyValues/css_value_border_image_source.h"
#include "API/CSSLayout/PropertyValues/css_value_border_image_slice.h"
#include "API/CSSLayout/PropertyValues/css_value_border_image_width.h"
#include "API/CSSLayout/PropertyValues/css_value_border_image_outset.h"
#include "API/CSSLayout/PropertyValues/css_value_border_image_repeat.h"

namespace clan
{

std::vector<std::string> CSSParserBorderImage::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-image");
	return names;
}

void CSSParserBorderImage::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueBorderImageSource> border_image_source(new CSSValueBorderImageSource());
	std::unique_ptr<CSSValueBorderImageSlice> border_image_slice(new CSSValueBorderImageSlice());
	std::unique_ptr<CSSValueBorderImageWidth> border_image_width(new CSSValueBorderImageWidth());
	std::unique_ptr<CSSValueBorderImageOutset> border_image_outset(new CSSValueBorderImageOutset());
	std::unique_ptr<CSSValueBorderImageRepeat> border_image_repeat(new CSSValueBorderImageRepeat());

	if (tokens.size() == 1 && tokens[0].type == CSSToken::type_ident && equals(tokens[0].value, "inherit"))
	{
		border_image_source->type = CSSValueBorderImageSource::type_inherit;
		border_image_slice->type = CSSValueBorderImageSlice::type_inherit;
		border_image_width->type = CSSValueBorderImageWidth::type_inherit;
		border_image_outset->type = CSSValueBorderImageOutset::type_inherit;
		border_image_repeat->type = CSSValueBorderImageRepeat::type_inherit;

		inout_values.push_back(std::move(border_image_source));
		inout_values.push_back(std::move(border_image_slice));
		inout_values.push_back(std::move(border_image_width));
		inout_values.push_back(std::move(border_image_outset));
		inout_values.push_back(std::move(border_image_repeat));

		return;
	}

	bool source_specified = false;
	bool slice_specified = false;
	bool repeat_specified = false;

	size_t pos = 0;
	do
	{
		if (!source_specified && parse_source(*border_image_source.get(), pos, tokens))
		{
			source_specified = true;
		}
		else if (!slice_specified && parse_slice(*border_image_slice.get(), pos, tokens))
		{
			slice_specified = true;

			size_t next_pos = pos;
			CSSToken token = next_token(next_pos, tokens);
			if (token.type == CSSToken::type_delim && token.value == "/")
			{
				pos = next_pos;
				if (parse_width(*border_image_width.get(), pos, tokens))
				{
					next_pos = pos;
					CSSToken token = next_token(next_pos, tokens);
					if (token.type == CSSToken::type_delim && token.value == "/")
					{
						pos = next_pos;
						if (!parse_outset(*border_image_outset.get(), pos, tokens))
						{
							return;
						}
					}
				}
				else if (!parse_outset(*border_image_outset.get(), pos, tokens))
				{
					return;
				}
			}
		}
		else if (!repeat_specified && parse_repeat(*border_image_repeat.get(), pos, tokens))
		{
			repeat_specified = true;
		}
		else
		{
			return;
		}
	} while (pos != tokens.size());

	inout_values.push_back(std::move(border_image_source));
	inout_values.push_back(std::move(border_image_slice));
	inout_values.push_back(std::move(border_image_width));
	inout_values.push_back(std::move(border_image_outset));
	inout_values.push_back(std::move(border_image_repeat));
}

bool CSSParserBorderImage::parse_source(CSSValueBorderImageSource &border_image_source, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "none"))
			border_image_source.type = CSSValueBorderImageSource::type_none;
		else
			return false;
	}
	else if (token.type == CSSToken::type_uri)
	{
		border_image_source.type = CSSValueBorderImageSource::type_image;
		border_image_source.url = token.value;
	}
	else
	{
		return false;
	}

	parse_pos = pos;
	return true;
}

bool CSSParserBorderImage::parse_slice(CSSValueBorderImageSlice &border_image_slice, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	size_t last_pos = pos;
	CSSToken token = next_token(pos, tokens);

	int num_lengths;
	CSSValueBorderImageSlice::ValueType value_types[4];
	float percentages[4] = { 0, 0, 0, 0 };
	float numbers[4] = { 0, 0, 0, 0 };
	for (num_lengths = 0; num_lengths < 4; num_lengths++)
	{
		if (token.type == CSSToken::type_percentage)
		{
			value_types[num_lengths] = CSSValueBorderImageSlice::value_type_percentage;
			percentages[num_lengths] = StringHelp::text_to_float(token.value);
		}
		else if (token.type == CSSToken::type_number)
		{
			value_types[num_lengths] = CSSValueBorderImageSlice::value_type_number;
			numbers[num_lengths] = StringHelp::text_to_float(token.value);
		}
		else
		{
			break;
		}

		last_pos = pos;
		token = next_token(pos, tokens);
	}

	bool fill_center = false;
	if (token.type == CSSToken::type_ident && equals(token.value, "fill"))
	{
		fill_center = true;
	}
	else
	{
		pos = last_pos;
	}

	if (num_lengths < 1)
		return false;

	border_image_slice.type = CSSValueBorderImageSlice::type_values;
	if (num_lengths == 1)
	{
		for (int i = 1; i < 4; i++)
		{
			value_types[i] = value_types[0];
			percentages[i] = percentages[0];
			numbers[i] = numbers[0];
		}
	}
	else if (num_lengths == 2)
	{
		value_types[2] = value_types[0];
		percentages[2] = percentages[0];
		numbers[2] = numbers[0];
		value_types[3] = value_types[1];
		percentages[3] = percentages[1];
		numbers[3] = numbers[1];
	}
	else if (num_lengths == 3)
	{
		value_types[3] = value_types[1];
		percentages[3] = percentages[1];
		numbers[3] = numbers[1];
	}

	border_image_slice.fill_center = fill_center;
	border_image_slice.value_top = value_types[0];
	border_image_slice.value_right = value_types[1];
	border_image_slice.value_bottom = value_types[2];
	border_image_slice.value_left = value_types[3];
	border_image_slice.percentage_top = percentages[0];
	border_image_slice.percentage_right = percentages[1];
	border_image_slice.percentage_bottom = percentages[2];
	border_image_slice.percentage_left = percentages[3];
	border_image_slice.number_top = numbers[0];
	border_image_slice.number_right = numbers[1];
	border_image_slice.number_bottom = numbers[2];
	border_image_slice.number_left = numbers[3];

	parse_pos = pos;
	return true;
}

bool CSSParserBorderImage::parse_width(CSSValueBorderImageWidth &border_image_width, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	size_t last_pos = pos;
	CSSToken token = next_token(pos, tokens);

	int num_lengths;
	CSSValueBorderImageWidth::ValueType value_types[4];
	CSSLength lengths[4];
	float percentages[4] = { 0, 0, 0, 0 };
	float numbers[4] = { 0, 0, 0, 0 };
	for (num_lengths = 0; num_lengths < 4; num_lengths++)
	{
		if (is_length(token))
		{
			value_types[num_lengths] = CSSValueBorderImageWidth::value_type_length;
			if (!parse_length(token, lengths[num_lengths]))
				return false;
		}
		else if (token.type == CSSToken::type_number)
		{
			value_types[num_lengths] = CSSValueBorderImageWidth::value_type_number;
			numbers[num_lengths] = StringHelp::text_to_float(token.value);
		}
		else if (token.type == CSSToken::type_percentage)
		{
			value_types[num_lengths] = CSSValueBorderImageWidth::value_type_percentage;
			percentages[num_lengths] = StringHelp::text_to_float(token.value);
		}
		else if (token.type == CSSToken::type_ident && equals(token.value, "auto"))
		{
			value_types[num_lengths] = CSSValueBorderImageWidth::value_type_auto;
		}
		else
		{
			break;
		}

		last_pos = pos;
		token = next_token(pos, tokens);
	}
	if (num_lengths < 1)
		return false;

	pos = last_pos;

	border_image_width.type = CSSValueBorderImageWidth::type_values;
	if (num_lengths == 1)
	{
		for (int i = 1; i < 4; i++)
		{
			value_types[i] = value_types[0];
			lengths[i] = lengths[0];
			numbers[i] = numbers[0];
			percentages[i] = percentages[0];
		}
	}
	else if (num_lengths == 2)
	{
		value_types[2] = value_types[0];
		lengths[2] = lengths[0];
		numbers[2] = numbers[0];
		percentages[2] = percentages[0];
		value_types[3] = value_types[1];
		lengths[3] = lengths[1];
		numbers[3] = numbers[1];
		percentages[3] = percentages[1];
	}
	else if (num_lengths == 3)
	{
		value_types[3] = value_types[1];
		lengths[3] = lengths[1];
		numbers[3] = numbers[1];
		percentages[3] = percentages[1];
	}

	border_image_width.value_top = value_types[0];
	border_image_width.value_right = value_types[1];
	border_image_width.value_bottom = value_types[2];
	border_image_width.value_left = value_types[3];
	border_image_width.length_top = lengths[0];
	border_image_width.length_right = lengths[1];
	border_image_width.length_bottom = lengths[2];
	border_image_width.length_left = lengths[3];
	border_image_width.number_top = numbers[0];
	border_image_width.number_right = numbers[1];
	border_image_width.number_bottom = numbers[2];
	border_image_width.number_left = numbers[3];
	border_image_width.percentage_top = percentages[0];
	border_image_width.percentage_right = percentages[1];
	border_image_width.percentage_bottom = percentages[2];
	border_image_width.percentage_left = percentages[3];

	parse_pos = pos;
	return true;
}

bool CSSParserBorderImage::parse_outset(CSSValueBorderImageOutset &border_image_outset, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	size_t last_pos = pos;
	CSSToken token = next_token(pos, tokens);

	int num_lengths;
	CSSValueBorderImageOutset::ValueType value_types[4];
	CSSLength lengths[4];
	float numbers[4] = { 0, 0, 0, 0 };
	for (num_lengths = 0; num_lengths < 4; num_lengths++)
	{
		if (is_length(token))
		{
			value_types[num_lengths] = CSSValueBorderImageOutset::value_type_length;
			if (!parse_length(token, lengths[num_lengths]))
				return false;
		}
		else if (token.type == CSSToken::type_number)
		{
			value_types[num_lengths] = CSSValueBorderImageOutset::value_type_number;
			numbers[num_lengths] = StringHelp::text_to_float(token.value);
		}
		else
		{
			break;
		}

		last_pos = pos;
		token = next_token(pos, tokens);
	}
	if (num_lengths < 1)
		return false;

	pos = last_pos;

	border_image_outset.type = CSSValueBorderImageOutset::type_values;
	if (num_lengths == 1)
	{
		for (int i = 1; i < 4; i++)
		{
			value_types[i] = value_types[0];
			lengths[i] = lengths[0];
			numbers[i] = numbers[0];
		}
	}
	else if (num_lengths == 2)
	{
		value_types[2] = value_types[0];
		lengths[2] = lengths[0];
		numbers[2] = numbers[0];
		value_types[3] = value_types[1];
		lengths[3] = lengths[1];
		numbers[3] = numbers[1];
	}
	else if (num_lengths == 3)
	{
		value_types[3] = value_types[1];
		lengths[3] = lengths[1];
		numbers[3] = numbers[1];
	}

	border_image_outset.value_top = value_types[0];
	border_image_outset.value_right = value_types[1];
	border_image_outset.value_bottom = value_types[2];
	border_image_outset.value_left = value_types[3];
	border_image_outset.length_top = lengths[0];
	border_image_outset.length_right = lengths[1];
	border_image_outset.length_bottom = lengths[2];
	border_image_outset.length_left = lengths[3];
	border_image_outset.number_top = numbers[0];
	border_image_outset.number_right = numbers[1];
	border_image_outset.number_bottom = numbers[2];
	border_image_outset.number_left = numbers[3];

	parse_pos = pos;

	return true;
}

bool CSSParserBorderImage::parse_repeat(CSSValueBorderImageRepeat &border_image_repeat, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && equals(token.value, "stretch"))
	{
		border_image_repeat.repeat_x = CSSValueBorderImageRepeat::repeat_type_stretch;
	}
	else if (token.type == CSSToken::type_ident && equals(token.value, "repeat"))
	{
		border_image_repeat.repeat_x = CSSValueBorderImageRepeat::repeat_type_repeat;
	}
	else if (token.type == CSSToken::type_ident && equals(token.value, "round"))
	{
		border_image_repeat.repeat_x = CSSValueBorderImageRepeat::repeat_type_round;
	}
	else
	{
		return false;
	}

	border_image_repeat.repeat_y = border_image_repeat.repeat_x;
	parse_pos = pos;

	if (pos != tokens.size())
	{
		token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident && equals(token.value, "stretch"))
		{
			border_image_repeat.repeat_y = CSSValueBorderImageRepeat::repeat_type_stretch;
			parse_pos = pos;
		}
		else if (token.type == CSSToken::type_ident && equals(token.value, "repeat"))
		{
			border_image_repeat.repeat_y = CSSValueBorderImageRepeat::repeat_type_repeat;
			parse_pos = pos;
		}
		else if (token.type == CSSToken::type_ident && equals(token.value, "round"))
		{
			border_image_repeat.repeat_y = CSSValueBorderImageRepeat::repeat_type_round;
			parse_pos = pos;
		}
	}
	return true;
}

}
