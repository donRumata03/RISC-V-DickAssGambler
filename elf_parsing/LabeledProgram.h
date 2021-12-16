//
// Created by Vova on 18.12.2021.
//

#pragma once

#include "pch.h"
#include "ElfFile.h"
#include "risc_v/Instruction.h"
#include "risc_v/command_parser.h"
#include "risc_v/base_instruction_formatter.h"

class LabeledProgram
{
	ElfFile file;
	std::vector<Instruction> instruction_sequence;
	std::unordered_map<u32, u32> line_of_code_by_non_labeled_jump_target;

	explicit LabeledProgram(ElfFile raw_file);

	std::string render_program();
	std::string render_instruction(const Instruction& instruction);
};


