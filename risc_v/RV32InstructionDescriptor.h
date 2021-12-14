//
// Created by Vova on 15.12.2021.
//

#pragma once

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
	FULL_MATCH_VALIDATION
};

struct RV32InstructionDescriptor
{
	std::string name;
	RV32InstructionPattern pattern;
	u32 opcode;

	std::optional<u32> funct3;
	std::optional<u32> funct7;
	std::optional<u32> match_after_opcode;
};


inline std::vector<RV32InstructionDescriptor> rv_32_instruction_descriptors {
		{ "lui",   RV32InstructionPattern::U, 0b0110111, {}, {} },
		{ "auipc", RV32InstructionPattern::U, 0b0010111, {}, {} },
		{ "jal",   RV32InstructionPattern::UJ, 0b1101111, {}, {} }, // TODO: Interface for viewing addresses
		{ "jalr",  RV32InstructionPattern::I, 0b1100111, 0b000, {} },

		{ "beq",   RV32InstructionPattern::SB, 0b1100011, 0b000, {} },
		{ "bne",   RV32InstructionPattern::SB, 0b1100011, 0b001, {} },
		{ "blt",   RV32InstructionPattern::SB, 0b1100011, 0b100, {} },
		{ "bge",   RV32InstructionPattern::SB, 0b1100011, 0b101, {} },
		{ "bltu",  RV32InstructionPattern::SB, 0b1100011, 0b110, {} }, // TODO: how do immediates work in unsigned commands
		{ "bgeu",  RV32InstructionPattern::SB, 0b1100011, 0b111, {} },

		{ "lb",    RV32InstructionPattern::I, 0b0000011, 0b000, {} },
		{ "lh",    RV32InstructionPattern::I, 0b0000011, 0b001, {} },
		{ "lw",    RV32InstructionPattern::I, 0b0000011, 0b010, {} },
		{ "lbu",   RV32InstructionPattern::I, 0b0000011, 0b100, {} },
		{ "lhu",   RV32InstructionPattern::I, 0b0000011, 0b101, {} },

		{ "sb",   RV32InstructionPattern::S, 0b0100011, 0b000, {} },
		{ "sh",   RV32InstructionPattern::S, 0b0100011, 0b001, {} },

		{ "addi",   RV32InstructionPattern::I, 0b0010011, 0b000, {} },
		{ "slti",   RV32InstructionPattern::I, 0b0010011, 0b010, {} },
		{ "sltiu",   RV32InstructionPattern::I, 0b0010011, 0b011, {} },
		{ "xori",   RV32InstructionPattern::I,  0b0010011, 0b100, {} },
		{ "ori",   RV32InstructionPattern::I, 0b0010011, 0b110, {} },
		{ "andi",   RV32InstructionPattern::I, 0b0010011, 0b111, {} },

		{ "slli",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b001, {} },
		{ "srli",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b101, 0b0000000 },
		{ "srai",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b101, 0b0100000 },

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

		{ "ecall",   RV32InstructionPattern::FULL_MATCH_VALIDATION, 0b1110011, {}, {}, 0b000000000000'00000'000'00000 },
		{ "ebreak",  RV32InstructionPattern::FULL_MATCH_VALIDATION, 0b1110011, {}, {}, 0b000000000001'00000'000'00000 },

		// TODO: CSR commands
		// { "",  RV32InstructionPattern::FULL_MATCH_VALIDATION, 0b1110011, {}, {}, 0b000000000001'00000'000'00000 },
};

