//
// Created by Vova on 15.12.2021.
//

#pragma once

#include <ostream>
#include "pch.h"

enum class RV32InstructionPattern
{
	R,
	I,
	I_SHIFT,
	S,
	SB,
	U,
	UJ,
	FULL_MATCH_VALIDATION,
	CSR_COMMAND,
	ZIMM_CSR_COMMAND
};

inline bool has_funct3(RV32InstructionPattern pattern) {
	return pattern != RV32InstructionPattern::U && pattern != RV32InstructionPattern::UJ;
}

inline bool has_funct7(RV32InstructionPattern pattern) {
	return pattern == RV32InstructionPattern::R;
}

inline bool has_rs1(RV32InstructionPattern pattern) {
	return pattern != RV32InstructionPattern::U && pattern != RV32InstructionPattern::UJ;
}

inline bool has_rs2(RV32InstructionPattern pattern) {
	return pattern == RV32InstructionPattern::R || pattern == RV32InstructionPattern::S || pattern == RV32InstructionPattern::SB;
}

inline bool has_rd(RV32InstructionPattern pattern) {
	return pattern != RV32InstructionPattern::S && pattern != RV32InstructionPattern::SB;
}


inline bool has_csr(RV32InstructionPattern pattern) {
	return pattern == RV32InstructionPattern::CSR_COMMAND && pattern == RV32InstructionPattern::ZIMM_CSR_COMMAND;
}



enum class ImmediateSignness
{
	SIGNED,
	UNSIGNED,
	ZERO_EXTENDED_UNSIGNED,
};

inline std::ostream& operator<< (std::ostream& os, const ImmediateSignness& opt) {
	switch (opt) {
		case ImmediateSignness::SIGNED:
			os << "SIGNED";
			break;
		case ImmediateSignness::UNSIGNED:
			os << "UNSIGNED";
			break;
		case ImmediateSignness::ZERO_EXTENDED_UNSIGNED:
			os << "ZERO_EXTENDED_UNSIGNED";
			break;
	}
	return os;
}



struct RV32InstructionDescriptor
{
	std::string name;
	RV32InstructionPattern pattern;
	u32 opcode;

	std::optional<u32> funct3; // For those who have it
	std::optional<u32> funct7; // For those who have it
	std::optional<ImmediateSignness> immediate_signedness; // For those who have immediates (all, excluding: { R, FULL_MATCH_VALIDATION, CSR_COMMAND }).
	std::optional<u32> match_after_opcode; // For full-matching variant
	bool contains_static_address_offset = false;

	bool operator== (const RV32InstructionDescriptor& rhs) const
	{
		return name == rhs.name &&
		       pattern == rhs.pattern &&
		       opcode == rhs.opcode &&
		       funct3 == rhs.funct3 &&
		       funct7 == rhs.funct7 &&
		       immediate_signedness == rhs.immediate_signedness &&
		       match_after_opcode == rhs.match_after_opcode &&
		       contains_static_address_offset == rhs.contains_static_address_offset;
	}

	bool operator!= (const RV32InstructionDescriptor& rhs) const
	{
		return !(rhs == *this);
	}

	friend std::ostream& operator<< (std::ostream& os, const RV32InstructionDescriptor& descriptor);
};


inline std::vector<RV32InstructionDescriptor> rv_32_instruction_descriptors {
		{ "lui",   RV32InstructionPattern::U, 0b0110111, {}, {}, ImmediateSignness::SIGNED, {}, false },
		{ "auipc", RV32InstructionPattern::U, 0b0010111, {}, {}, ImmediateSignness::SIGNED, {}, false },
		{ "jal",   RV32InstructionPattern::UJ, 0b1101111, {}, {}, ImmediateSignness::SIGNED, {}, true},
		{ "jalr",  RV32InstructionPattern::I, 0b1100111, 0b000, {}, ImmediateSignness::SIGNED, {}, false },

		{ "beq",   RV32InstructionPattern::SB, 0b1100011, 0b000, {}, ImmediateSignness::SIGNED, {}, true },
		{ "bne",   RV32InstructionPattern::SB, 0b1100011, 0b001, {}, ImmediateSignness::SIGNED, {}, true },
		{ "blt",   RV32InstructionPattern::SB, 0b1100011, 0b100, {}, ImmediateSignness::SIGNED, {}, true },
		{ "bge",   RV32InstructionPattern::SB, 0b1100011, 0b101, {}, ImmediateSignness::SIGNED, {}, true },
		{ "bltu",  RV32InstructionPattern::SB, 0b1100011, 0b110, {}, ImmediateSignness::SIGNED, {}, true },
		{ "bgeu",  RV32InstructionPattern::SB, 0b1100011, 0b111, {}, ImmediateSignness::SIGNED, {}, true },

		{ "lb",    RV32InstructionPattern::I, 0b0000011, 0b000, {}, ImmediateSignness::SIGNED },
		{ "lh",    RV32InstructionPattern::I, 0b0000011, 0b001, {}, ImmediateSignness::SIGNED },
		{ "lw",    RV32InstructionPattern::I, 0b0000011, 0b010, {}, ImmediateSignness::SIGNED },
		{ "lbu",   RV32InstructionPattern::I, 0b0000011, 0b100, {}, ImmediateSignness::SIGNED },
		{ "lhu",   RV32InstructionPattern::I, 0b0000011, 0b101, {}, ImmediateSignness::SIGNED },

		{ "sb",   RV32InstructionPattern::S, 0b0100011, 0b000, {}, ImmediateSignness::SIGNED },
		{ "sh",   RV32InstructionPattern::S, 0b0100011, 0b001, {}, ImmediateSignness::SIGNED },
		{ "sw",   RV32InstructionPattern::S, 0b0100011, 0b010, {}, ImmediateSignness::SIGNED },

		{ "addi",   RV32InstructionPattern::I, 0b0010011, 0b000, {}, ImmediateSignness::SIGNED },
		{ "slti",   RV32InstructionPattern::I, 0b0010011, 0b010, {}, ImmediateSignness::SIGNED },
		{ "sltiu",   RV32InstructionPattern::I, 0b0010011, 0b011, {}, ImmediateSignness::UNSIGNED },
		{ "xori",   RV32InstructionPattern::I,  0b0010011, 0b100, {}, ImmediateSignness::SIGNED },
		{ "ori",   RV32InstructionPattern::I, 0b0010011, 0b110, {}, ImmediateSignness::SIGNED },
		{ "andi",   RV32InstructionPattern::I, 0b0010011, 0b111, {}, ImmediateSignness::SIGNED },

		{ "slli",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b001, {}, ImmediateSignness::ZERO_EXTENDED_UNSIGNED },
		{ "srli",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b101, 0b0000000, ImmediateSignness::ZERO_EXTENDED_UNSIGNED },
		{ "srai",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b101, 0b0100000, ImmediateSignness::ZERO_EXTENDED_UNSIGNED },

		{ "add",   RV32InstructionPattern::R, 0b0110011, 0b000, 0b0000000 },
		{ "sub",   RV32InstructionPattern::R, 0b0110011, 0b000, 0b0100000 },
		{ "sll",   RV32InstructionPattern::R, 0b0110011, 0b001, 0b0000000 },
		{ "slt",   RV32InstructionPattern::R, 0b0110011, 0b010, 0b0000000 },
		{ "sltu",  RV32InstructionPattern::R, 0b0110011, 0b011, 0b0000000 },
		{ "xor",   RV32InstructionPattern::R, 0b0110011, 0b100, 0b0000000 },
		{ "srl",   RV32InstructionPattern::R, 0b0110011, 0b101, 0b0000000 },
		{ "sra",   RV32InstructionPattern::R, 0b0110011, 0b101, 0b0100000 },
		{ "sra",   RV32InstructionPattern::R, 0b0110011, 0b110, 0b0000000 },
		{ "add",   RV32InstructionPattern::R, 0b0110011, 0b111, 0b0000000 },

		{ "ecall",   RV32InstructionPattern::FULL_MATCH_VALIDATION, 0b1110011, {}, {}, {}, 0b000000000000'00000'000'00000 },
		{ "ebreak",  RV32InstructionPattern::FULL_MATCH_VALIDATION, 0b1110011, {}, {}, {}, 0b000000000001'00000'000'00000 },

		{ "csrrw",  RV32InstructionPattern::CSR_COMMAND, 0b1110011, 0b001, {} },
		{ "csrrs",  RV32InstructionPattern::CSR_COMMAND, 0b1110011, 0b010, {} },
		{ "csrrc",  RV32InstructionPattern::CSR_COMMAND, 0b1110011, 0b011, {} },
		{ "csrwi",  RV32InstructionPattern::ZIMM_CSR_COMMAND, 0b1110011, 0b101, {}, ImmediateSignness::ZERO_EXTENDED_UNSIGNED },
		{ "csrsi",  RV32InstructionPattern::ZIMM_CSR_COMMAND, 0b1110011, 0b110, {}, ImmediateSignness::ZERO_EXTENDED_UNSIGNED },
		{ "csrci",  RV32InstructionPattern::ZIMM_CSR_COMMAND, 0b1110011, 0b111, {}, ImmediateSignness::ZERO_EXTENDED_UNSIGNED },


		{ "mul",   RV32InstructionPattern::R, 0b0110011, 0b000, 0b0000001 },
		{ "mulh",   RV32InstructionPattern::R, 0b0110011, 0b001, 0b0000001 },
		{ "mulhsu",   RV32InstructionPattern::R, 0b0110011, 0b010, 0b0000001 },
		{ "mulhu",   RV32InstructionPattern::R, 0b0110011, 0b011, 0b0000001 },

		{ "div",   RV32InstructionPattern::R, 0b0110011, 0b100, 0b0000001 },
		{ "divu",   RV32InstructionPattern::R, 0b0110011, 0b101, 0b0000001 },
		{ "rem",   RV32InstructionPattern::R, 0b0110011, 0b110, 0b0000001 },
		{ "remu",   RV32InstructionPattern::R, 0b0110011, 0b111, 0b0000001 },
};

