//
// Created by Vova on 14.12.2021.
//

#include "command_parser.h"
#include "generic_utils/string_viewing_utils.h"
#include "rv32_parser.h"
#include "rvc_parser.h"


std::vector<Instruction> parseInstructions (const byte_string& text_section, usize start_address)
{
	std::vector<Instruction> res;
	usize section_ptr = 0;

	while (section_ptr < text_section.size()) {
		switch (text_section[section_ptr] & 0b11) {
			case 0b11: {
				try {
					auto command = view_as_integral<u32>(byte_view { text_section }, section_ptr);
					auto instruction = parse_RV32_instruction(command);

//					if (command == 1939) {
//						assert(instruction.immediate);
//						auto val = instruction.immediate.value();
//						std::cout << std::get<i32>(val) << std::endl;
//					}

					res.push_back(instruction);
					auto& back = res.back();
					auto& back_addr = back.address;
					auto this_address = section_ptr + start_address;
					back_addr = this_address;
				} catch(std::exception& e) {
					std::cout << "Exception occurred: " << e.what() << std::endl;
					res.push_back({.address = static_cast<u32>(section_ptr + start_address)});
				}
				section_ptr += 4;
				break;
			}
			default:
				try {
					res.emplace_back(parse_RVC_instruction(view_as_integral<u16>(byte_view { text_section }, section_ptr)));
					res.back().address = section_ptr + start_address;
					std::cout << "16-bit RVC command!" << std::endl;
				} catch(std::exception& e) {
					res.push_back({.address = static_cast<u32>(section_ptr + start_address)});
				}

				section_ptr += 2;
		}
	}

	for (auto& instr : res) {
		if (instr.immediate) {
			assert(!instr.immediate->valueless_by_exception());
		}
	}
	std::cout << res << std::endl;

	return res;
}
