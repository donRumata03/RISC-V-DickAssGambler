//
// Created by Vova on 16.12.2021.
//

#include <generic_utils/formatting_utils.h>
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

		default:
			throw std::runtime_error("Invalid ImmediateSignness");
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

std::optional<Immediate> maybe_parse_immediate (u32 command, const RV32InstructionDescriptor& instruction_descriptor)
{
	try {
		return parse_immediate(command, instruction_descriptor);
	} catch (std::exception& e) {
		return {};
	}
}

Instruction parse_RV32_instruction (u32 command)
{
	// Determine command: firstly, by opcode:
	auto opcode = parse_opcode(command);
	std::vector<RV32InstructionDescriptor> matching_opcode;
	std::copy_if(rv_32_instruction_descriptors.begin(), rv_32_instruction_descriptors.end(), std::back_inserter(matching_opcode), [&](auto& d){
		return d.opcode == opcode;
	});

	if (matching_opcode.empty()) {
		throw std::runtime_error("Can't find command with opcode: " + format_hex(opcode));
	}

	// Try to match full pattern:
	// If fully matches with FULL_MATCH_VALIDATION
	std::vector<RV32InstructionDescriptor> matching_full_pattern;
	std::copy_if(matching_opcode.begin(), matching_opcode.end(), std::back_inserter(matching_full_pattern), [&](RV32InstructionDescriptor& d){
		return d.pattern == RV32InstructionPattern::FULL_MATCH_VALIDATION
		       && d.match_after_opcode == parse_excluding_opcode(command);
	});

	if (!matching_full_pattern.empty()) {
		assert(matching_full_pattern.size() == 1);
		return Instruction {
				.descriptor = matching_full_pattern[0]
		};
	}


	// If can't match full pattern:
	RV32InstructionDescriptor descriptor;
	if (matching_opcode.size() == 1) {
		descriptor = matching_opcode[0];
	} else {
		// Try to match by funct3
		auto funct3 = parse_funct3(command);
		std::vector<RV32InstructionDescriptor> matching_funct3;
		std::copy_if(matching_opcode.begin(), matching_opcode.end(), std::back_inserter(matching_funct3), [&](RV32InstructionDescriptor& d){
			if (!d.funct3) {
				throw std::runtime_error("Ambiguous command");
			}
			return *d.funct3 == funct3;
		});

		if (matching_funct3.empty()) {
			throw std::runtime_error("Can't match funct3");
		}

		if (matching_funct3.size() == 1) {
			descriptor = matching_funct3[0];
		} else {
			// Try to match by funct7
			auto funct7 = parse_funct7(command);
			std::vector<RV32InstructionDescriptor> matching_funct7;
			std::copy_if(matching_funct3.begin(), matching_funct3.end(), std::back_inserter(matching_funct7), [&](RV32InstructionDescriptor& d){
				if (!d.funct7) {
					throw std::runtime_error("Ambiguous command");
				}
				return *d.funct7 == funct7;
			});

			if (matching_funct7.empty()) {
				throw std::runtime_error("Can't match funct7");
			}
			descriptor = matching_funct7[0];
		}
	}


	return Instruction{
		.address = 0,
		.instruction_set = InstructionSet::RV32,
		.descriptor = descriptor,

		.src_register_left = has_rs1(descriptor.pattern) ? std::optional{IntRegister(parse_rs1(command))} : std::nullopt,
		.src_register_right = has_rs2(descriptor.pattern) ? std::optional{IntRegister(parse_rs2(command))} : std::nullopt,
		.dest_register = has_rd(descriptor.pattern) ? std::optional{ IntRegister(parse_rd(command))} : std::nullopt,

		.immediate = maybe_parse_immediate(command, descriptor),
		.csr_register = has_csr(descriptor.pattern) ? std::optional{ CsrRegister(parse_csr(command))} : std::nullopt,
	};
}

u32 parse_opcode (u32 command)
{
	return command & generate_trailing_ones(7);
}

u32 parse_excluding_opcode (u32 command)
{
	return uint_from_range_sequence(command, {{7, 31}});
}

u32 parse_funct3 (u32 command)
{
	return uint_from_range_sequence(command, {{12, 14}});
}

u32 parse_funct7 (u32 command)
{
	return uint_from_range_sequence(command, {{25, 31}});
}

u32 parse_rs1 (u32 command)
{
	return uint_from_range_sequence(command, {{15, 19}});
}

u32 parse_rs2 (u32 command)
{
	return uint_from_range_sequence(command, {{20, 24}});
}

u32 parse_rd (u32 command)
{
	return uint_from_range_sequence(command, {{7, 11}});
}

u32 parse_csr (u32 command)
{
	return uint_from_range_sequence(command, {{12, 14}});
}

