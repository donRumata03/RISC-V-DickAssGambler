//
// Created by vova on 11.12.2021.
//


#include "formatting_utils.h"

std::string format_hex_sequence (std::basic_string_view<u8> bytes)
{
	std::vector<std::string> strings(bytes.size());
	std::transform(bytes.begin(), bytes.end(), strings.begin(), [](u8 character){
		return format_hex_prefixless<u16, 2>(u16(character));
	});

	return join(strings, " ");
}
