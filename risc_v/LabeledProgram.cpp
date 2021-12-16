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

std::optional<std::string> LabeledProgram::get_label_for_address (u32 address)
{
	std::string symbol_name;
	if (file.get_symbol_by_address(address)) {
		return (*file.get_symbol_by_address(address)).name;
	}

	auto loc_label_try = line_of_code_by_non_labeled_jump_target.find(address);
	if (loc_label_try != line_of_code_by_non_labeled_jump_target.end()) {
		auto loc_number = *loc_label_try;
		return string_format("LOC_%05x", loc_number);
	}

	return {};
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
			formatted_immediate = *get_label_for_address(*instruction.maybe_get_jmp_address());
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
	std::vector<std::string> formatted_lines(instruction_sequence.size());

	for (const auto& instruction: instruction_sequence) {
		std::string formatted_instruction = bool(instruction.descriptor) ?
		                                    render_instruction(instruction) :
		                                    std::string("unknown-command");

		auto maybe_label = get_label_for_address(instruction.address);

		formatted_lines.push_back(bool(maybe_label) ?
				string_format("%08x %10s %s", instruction.address, "", formatted_instruction.c_str()) :
				string_format("%08x %10s: %s", instruction.address, (*maybe_label).c_str(), formatted_instruction.c_str())
				);
	}

	return join(formatted_lines, "\n");
}
