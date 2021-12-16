//
// Created by Vova on 18.12.2021.
//

#pragma once

#include "pch.h"
#include "elf_parsing/ElfFile.h"
#include "risc_v/Instruction.h"
#include "risc_v/command_parser.h"
#include "risc_v/base_instruction_formatter.h"

class LabeledProgram
{
private:
	ElfFile file;
	std::vector<Instruction> instruction_sequence;
	std::unordered_map<u32, u32> line_of_code_by_non_labeled_jump_target;

public:
	explicit LabeledProgram(ElfFile raw_file);
	std::string render_program();

private:
	std::optional<std::string> get_label_for_address(u32 address);

	std::string render_instruction(const Instruction& instruction);

};


