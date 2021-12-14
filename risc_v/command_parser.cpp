//
// Created by Vova on 14.12.2021.
//

#include "command_parser.h"
#include "generic_utils/string_viewing_utils.h"

Immediate parse_immediate (u32 command, const std::vector<std::pair<usize, usize>>& ranges, ImmediateSignness signedness)
{
	switch (signedness) {
		case ImmediateSignness::SIGNED:
			return two_s_complement_int_from_range_sequence(command, ranges);

		case ImmediateSignness::UNSIGNED:
			return static_cast<u32>(two_s_complement_int_from_range_sequence(command, ranges));

		case ImmediateSignness::ZERO_EXTENDED_UNSIGNED:
			return uint_from_range_sequence(command, ranges);
	}
}

Immediate parse_immediate (u32 command, const RV32InstructionDescriptor& instruction_descriptor)
{
	switch (instruction_descriptor.pattern) {
		using enum RV32InstructionPattern;

		case I: return parse_immediate(command, {
					{ 20, 31 }
			}, *instruction_descriptor.immediate_signedness);

		case I_SHIFT: return parse_immediate(command, {
					{}
			}, *instruction_descriptor.immediate_signedness);

		case S: return parse_immediate(command, {
					{ 7, 11 }, { 25, 31 }
			}, *instruction_descriptor.immediate_signedness);

		case SB: return shifted_left(parse_immediate(command, {
					{ 8, 11 }, { 25, 30 }, { 7, 7 }, { 31, 31 }
			}, *instruction_descriptor.immediate_signedness), 1);

		case U: return shifted_left(parse_immediate(command, {
					{ 12, 31 }
			}, *instruction_descriptor.immediate_signedness), 12);

		case UJ: return shifted_left(parse_immediate(command, {
					{ 21, 30 }, { 20, 20 }, { 12, 19 }, { 31, 31 }
			}, *instruction_descriptor.immediate_signedness), 1);

		case ZIMM_CSR_COMMAND: return parse_immediate(command, {
					{ 15, 19 }
			}, *instruction_descriptor.immediate_signedness);

		default:
			throw std::runtime_error("Can't parse immediates from other command types");
	}

}

Instruction parse_RV32_instruction (u32 command)
{
	return Instruction{};
}

Instruction parse_RVC_instruction (u16 command)
{
	return Instruction{};
}


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
