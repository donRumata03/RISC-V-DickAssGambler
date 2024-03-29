//
// Created by Vova on 16.12.2021.
//

#include <generic_utils/formatting_utils.h>
#include "base_instruction_formatter.h"
#include "elf_parsing/ElfFile.h"

#define just_case(name) case name: return #name;
#define just_IR_case(name) case IntRegister:: name: return #name;
#define just_CR_case(name) case CsrRegister:: name: return #name;

std::string get_int_register_name (IntRegister int_register)
{
	switch (int_register) {
		just_IR_case(zero)
		just_IR_case(ra)
		just_IR_case(sp)
		just_IR_case(gp)
		just_IR_case(tp)
		just_IR_case(t0)
		just_IR_case(t1)
		just_IR_case(t2)
		just_IR_case(fp)
		just_IR_case(s1)
		case IntRegister::a0: case IntRegister::a1: case IntRegister::a2: case IntRegister::a3: case IntRegister::a4: case IntRegister::a5: case IntRegister::a6: case IntRegister::a7:
			return "a" + std::to_string(u32(int_register) - u32(IntRegister::a0));
		case IntRegister::s2: case IntRegister::s3: case IntRegister::s4: case IntRegister::s5: case IntRegister::s6: case IntRegister::s7: case IntRegister::s8: case IntRegister::s9: case IntRegister::s10: case IntRegister::s11:
			return "s" + std::to_string(u32(int_register) + 2 - u32(IntRegister::s2));
		case IntRegister::t3: case IntRegister::t4: case IntRegister::t5: case IntRegister::t6:
			return "t" + std::to_string(u32(int_register) + 3 - u32(IntRegister::t3));
		default:
			throw std::runtime_error("Unknown register");
	}
}

std::string get_csr_register_name (CsrRegister int_register)
{
	switch (int_register) {
		just_CR_case(fflags)
		just_CR_case(frm)
		just_CR_case(fcsr)
		just_CR_case(cycle)
		just_CR_case(time)
		just_CR_case(instret)
		just_CR_case(cycleh)
		just_CR_case(timeh)
		just_CR_case(instreth)
	}

	return std::to_string(u32(int_register));
}

#undef just_case

std::string format_decimal_immediate (Immediate immediate)
{
	if (std::holds_alternative<u32>(immediate)) {
		return std::to_string(std::get<u32>(immediate));
	}

	return std::to_string(std::get<i32>(immediate));
}

std::string format_instruction (const Instruction& instruction)
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
		formatted_arguments.push_back(format_decimal_immediate(*instruction.immediate));
	}
	if (instruction.csr_register) {
		formatted_arguments.push_back(get_csr_register_name(*instruction.csr_register));
	}

	return instruction.descriptor->name + (formatted_arguments.empty() ?
		"" :
		" " + join(formatted_arguments, ", ")
	);
}

/////////////////  /////////////////  /////////////////  /////////////////  /////////////////  /////////////////  /////////////////

std::optional<std::string> get_label_for_address (
		const ElfFile& file,
		const std::unordered_map<u32, u32>& line_of_code_by_non_labeled_jump_target,
		u32 address
)
{
	std::string symbol_name;
	if (file.get_symbol_by_address(address)) {
		return (*file.get_symbol_by_address(address)).name;
	}

	auto loc_label_try = line_of_code_by_non_labeled_jump_target.find(address);
	if (loc_label_try != line_of_code_by_non_labeled_jump_target.end()) {
		auto loc_number = *loc_label_try;
		return string_format("LOC_%05d", loc_number.second);
	}

	return {};
}

std::string render_instruction (
		const ElfFile& file,
        const std::unordered_map<u32, u32>& line_of_code_by_non_labeled_jump_target,
		const Instruction& instruction
		)
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
			formatted_immediate = *get_label_for_address(file, line_of_code_by_non_labeled_jump_target, jmp_address);
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

std::string render_program (
		const ElfFile& file,
		const std::vector<Instruction>& instruction_sequence,
		const std::unordered_map<u32, u32>& line_of_code_by_non_labeled_jump_target
) {
	std::vector<std::string> formatted_lines;
	formatted_lines.reserve(instruction_sequence.size());

	for (const auto& instruction: instruction_sequence) {
		std::string formatted_instruction = bool(instruction.descriptor) ?
		                                    render_instruction(file, line_of_code_by_non_labeled_jump_target, instruction) :
		                                    std::string("unknown-command");

		auto maybe_label = get_label_for_address(file, line_of_code_by_non_labeled_jump_target, instruction.address);

		formatted_lines.push_back(bool(maybe_label) ?
		                          string_format("%08x %10s: %s", instruction.address, (*maybe_label).c_str(), formatted_instruction.c_str()) :
		                          string_format("%08x %10s %s", instruction.address, "", formatted_instruction.c_str())
		);
	}

	return join(formatted_lines, "\n");
}


std::string format_instructions (const ElfFile& file, const std::vector<Instruction>& instruction_sequence)
{
	/// First, compute non-labeled targets:

	std::unordered_map<u32, u32> instruction_index_by_address;
	for (usize line_of_code = 0; line_of_code < instruction_sequence.size(); ++line_of_code) {
		auto instruction = instruction_sequence[line_of_code];
		instruction_index_by_address[instruction.address] = line_of_code;
	}

	std::unordered_map<u32, u32> line_of_code_by_non_labeled_jump_target;

	for (usize line_of_code = 0; line_of_code < instruction_sequence.size(); ++line_of_code) {
		auto instruction = instruction_sequence[line_of_code];

		if (instruction.maybe_get_jmp_address()) {
			auto jmp_address = *instruction.maybe_get_jmp_address();
			if (!file.get_symbol_by_address(jmp_address)) {
				line_of_code_by_non_labeled_jump_target[jmp_address] =
						instruction_index_by_address[jmp_address];
			}
		}
	}

	return render_program(file, instruction_sequence, line_of_code_by_non_labeled_jump_target);
}

std::string format_non_labeled_instructions (const std::vector<Instruction>& instructions)
{
	std::vector<std::string> formatted(instructions.size());
	std::transform(instructions.begin(), instructions.end(), formatted.begin(), [](const Instruction& instruction){
		std::string formatted_instruction = bool(instruction.descriptor) ?
		                                    format_instruction(instruction) :
		                                    std::string("unknown-command");

		return string_format("%08x %10s %s", instruction.address, "", formatted_instruction.c_str());
	});

	return join(formatted, "\n");
}
