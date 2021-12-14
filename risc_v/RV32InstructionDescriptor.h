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
	FULL_MATCH_VALIDATION,
	CSR_COMMAND,
	ZIMM_CSR_COMMAND,
};

enum class ImmediateSignness
{
	SIGNED,
	UNSIGNED,
	ZERO_EXTENDED_UNSIGNED,
};

struct RV32InstructionDescriptor
{
	std::string name;
	RV32InstructionPattern pattern;
	u32 opcode;

	std::optional<u32> funct3; // For those who have it
	std::optional<u32> funct7; // For those who have it
	std::optional<ImmediateSignness> immediate_signedness; // For those who have immediates (all, excluding: { R, FULL_MATCH_VALIDATION, CSR_COMMAND }).
	std::optional<u32> match_after_opcode; // For full-matching variant
};


inline std::vector<RV32InstructionDescriptor> rv_32_instruction_descriptors {
		{ "lui",   RV32InstructionPattern::U, 0b0110111, {}, {}, ImmediateSignness::UNSIGNED },
		{ "auipc", RV32InstructionPattern::U, 0b0010111, {}, {}, ImmediateSignness::UNSIGNED },
		{ "jal",   RV32InstructionPattern::UJ, 0b1101111, {}, {}, ImmediateSignness::SIGNED}, // TODO: Interface for viewing addresses
		{ "jalr",  RV32InstructionPattern::I, 0b1100111, 0b000, {}, ImmediateSignness::SIGNED },

		{ "beq",   RV32InstructionPattern::SB, 0b1100011, 0b000, {}, ImmediateSignness::SIGNED },
		{ "bne",   RV32InstructionPattern::SB, 0b1100011, 0b001, {}, ImmediateSignness::SIGNED },
		{ "blt",   RV32InstructionPattern::SB, 0b1100011, 0b100, {}, ImmediateSignness::SIGNED },
		{ "bge",   RV32InstructionPattern::SB, 0b1100011, 0b101, {}, ImmediateSignness::SIGNED },
		{ "bltu",  RV32InstructionPattern::SB, 0b1100011, 0b110, {}, ImmediateSignness::SIGNED }, // TODO: how do immediates work in unsigned commands
		{ "bgeu",  RV32InstructionPattern::SB, 0b1100011, 0b111, {}, ImmediateSignness::SIGNED },

		{ "lb",    RV32InstructionPattern::I, 0b0000011, 0b000, {}, ImmediateSignness::SIGNED },
		{ "lh",    RV32InstructionPattern::I, 0b0000011, 0b001, {}, ImmediateSignness::SIGNED },
		{ "lw",    RV32InstructionPattern::I, 0b0000011, 0b010, {}, ImmediateSignness::SIGNED },
		{ "lbu",   RV32InstructionPattern::I, 0b0000011, 0b100, {}, ImmediateSignness::UNSIGNED },
		{ "lhu",   RV32InstructionPattern::I, 0b0000011, 0b101, {}, ImmediateSignness::UNSIGNED },

		{ "sb",   RV32InstructionPattern::S, 0b0100011, 0b000, {}, ImmediateSignness::SIGNED },
		{ "sh",   RV32InstructionPattern::S, 0b0100011, 0b001, {}, ImmediateSignness::SIGNED },

		{ "addi",   RV32InstructionPattern::I, 0b0010011, 0b000, {}, ImmediateSignness::SIGNED },
		{ "slti",   RV32InstructionPattern::I, 0b0010011, 0b010, {}, ImmediateSignness::SIGNED },
		{ "sltiu",   RV32InstructionPattern::I, 0b0010011, 0b011, {}, ImmediateSignness::UNSIGNED },
		{ "xori",   RV32InstructionPattern::I,  0b0010011, 0b100, {}, ImmediateSignness::SIGNED },
		{ "ori",   RV32InstructionPattern::I, 0b0010011, 0b110, {}, ImmediateSignness::SIGNED },
		{ "andi",   RV32InstructionPattern::I, 0b0010011, 0b111, {}, ImmediateSignness::SIGNED },

		{ "slli",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b001, {}, ImmediateSignness::UNSIGNED },
		{ "srli",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b101, 0b0000000, ImmediateSignness::UNSIGNED },
		{ "srai",   RV32InstructionPattern::I_SHIFT, 0b0010011, 0b101, 0b0100000, ImmediateSignness::UNSIGNED },

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
};

