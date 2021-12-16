//
// Created by Vova on 18.12.2021.
//

#pragma once

#include "pch.h"
#include "ElfFile.h"
#include "risc_v/Instruction.h"
#include "risc_v/command_parser.h"

class LabeledProgram
{
	ElfFile file;
	std::vector<Instruction> instruction_sequence;
	std::unordered_set<u32> non_labeled_jump_targets;

	explicit LabeledProgram(ElfFile raw_file)
		: file(std::move(raw_file))
	{
		instruction_sequence = parseInstructions(file.text_section.data, file.text_section.header.virtual_address);

		for (const auto& instruction: instruction_sequence) {
			if (instruction.maybe_get_address()) {
				auto jmp_address = *instruction.maybe_get_address();
				if (file.get_symbol_by_address(jmp_address)) {
					non_labeled_jump_targets.insert(jmp_address);
				}
			}
		}
	}

	std::string render() {
		return ""; // TODO
	}
};


