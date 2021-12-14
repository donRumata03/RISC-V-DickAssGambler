//
// Created by Vova on 14.12.2021.
//

#include "command_parser.h"
#include "generic_utils/string_viewing_utils.h"



std::vector<Instruction> parseInstructions (const byte_string& text_section, usize start_address)
{
	std::vector<Instruction> res;
	usize section_ptr = 0;

	while (section_ptr < text_section.size()) {
		switch (text_section[section_ptr] & 0b11) {
			case 0b11: {
				res.push_back(parse_RV32_instruction(view_as_integral<u32>(text_section, section_ptr)));
				res.back().address = section_ptr + start_address;

				section_ptr += 4;
			}
			default:
				res.push_back(parse_RVC_instruction(view_as_integral<u16>(text_section, section_ptr)));
				res.back().address = section_ptr + start_address;

				section_ptr += 2;
		}
	}

	return res;
}
