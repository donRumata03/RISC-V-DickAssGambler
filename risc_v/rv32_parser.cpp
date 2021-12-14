//
// Created by Vova on 16.12.2021.
//

#include "rv32_parser.h"


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
					{ 20, 24 }
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

u32 parse_funct3 (u32 command)
{
	return 0;
}

u32 parse_funct7 (u32 command)
{
	return 0;
}

u32 parse_rs1 (u32 command)
{
	return 0;
}

u32 parse_rs2 (u32 command)
{
	return 0;
}

u32 parse_rd (u32 command)
{
	return 0;
}

