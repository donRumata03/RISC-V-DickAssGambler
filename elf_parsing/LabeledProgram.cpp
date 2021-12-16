//
// Created by Vova on 18.12.2021.
//

#include "LabeledProgram.h"

LabeledProgram::LabeledProgram (ElfFile raw_file)
		: file(std::move(raw_file))
{
	instruction_sequence = parseInstructions(file.text_section.data, file.text_section.header.virtual_address);

	usize line_of_code = 0;
	for (const auto& instruction: instruction_sequence) {
		if (instruction.maybe_get_jmp_address()) {
			auto jmp_address = *instruction.maybe_get_jmp_address();
			if (file.get_symbol_by_address(jmp_address)) {
				line_of_code_by_non_labeled_jump_target[jmp_address] = line_of_code;
			}
		}

		line_of_code++;
	}
}

std::string LabeledProgram::render_instruction (const Instruction& instruction)
{
	std::vector<std::string> formatted_arguments;

	if (instruction.dest_register) {
		formatted_arguments.push_back(get_int_register_name(*instruction.dest_register));
	}
	if (instruction.src_register_left) {
		formatted_arguments.push_back(get_int_register_name(*instruction.src_register_left));
	}
	if (instruction.src_register_right) {
		formatted_arguments.push_back(get_int_register_name(*instruction.src_register_right));
	}
	if (instruction.immediate) {
		std::string formatted_immediate;

		if (instruction.maybe_get_jmp_address()) {
			// Immediate should be formatted with address
			auto jmp_address = *instruction.maybe_get_jmp_address();
			std::string symbol_name;
			if (file.get_symbol_by_address(jmp_address)) {
				symbol_name = (*file.get_symbol_by_address(jmp_address)).name;
			} else {
				auto loc_number = line_of_code_by_non_labeled_jump_target[jmp_address];
				symbol_name = string_format("LOC_%05x", loc_number);
			}

			formatted_immediate = std::move(symbol_name);
		} else {
			formatted_immediate = format_decimal_immediate(*instruction.immediate);
		}

		formatted_arguments.push_back(formatted_immediate);
	}
	if (instruction.csr_register) {
		formatted_arguments.push_back(get_csr_register_name(*instruction.csr_register));
	}

	return instruction.descriptor->name + (formatted_arguments.empty() ?
	                                       "" :
	                                       " " + join(formatted_arguments, ", ")
	);
}

std::string LabeledProgram::render_program ()
{
	std::vector<std::string> formatted(instruction_sequence.size());

	for (const auto& instruction: instruction_sequence) {
		formatted.push_back(render_instruction(instruction));
	}

	return join(formatted, "\n");
}
