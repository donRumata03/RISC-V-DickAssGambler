//
// Created by Vova on 16.12.2021.
//

#include <generic_utils/formatting_utils.h>
#include "base_instruction_formatter.h"

#define just_case(name) case name: return #name;

std::string get_int_register_name (IntRegister int_register)
{
	switch (int_register) {
		using enum IntRegister;

		just_case(zero)
		just_case(ra)
		just_case(sp)
		just_case(gp)
		just_case(tp)
		just_case(t0)
		just_case(t1)
		just_case(t2)
		just_case(fp)
		just_case(s1)
		case a0: case a1: case a2: case a3: case a4: case a5: case a6: case a7:
			return "a" + std::to_string(u32(int_register) - u32(a0));
		case s2: case s3: case s4: case s5: case s6: case s7: case s8: case s9: case s10: case s11:
			return "s" + std::to_string(u32(int_register) + 2 - u32(s2));
		case t3: case t4: case t5: case t6:
			return "t" + std::to_string(u32(int_register) + 3 - u32(t3));
	}
}

std::string get_csr_register_name (CsrRegister int_register)
{
	switch (int_register) {
		using enum CsrRegister;

		just_case(fflags)
		just_case(frm)
		just_case(fcsr)
		just_case(cycle)
		just_case(time)
		just_case(instret)
		just_case(cycleh)
		just_case(timeh)
		just_case(instreth)
	}

	return std::to_string(u32(int_register));
}

#undef just_case

std::string format_immediate (Immediate immediate)
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
		formatted_arguments.push_back(format_immediate(*instruction.immediate));
	}
	if (instruction.csr_register) {
		formatted_arguments.push_back(get_csr_register_name(*instruction.csr_register));
	}

	return instruction.descriptor->name + (formatted_arguments.empty() ?
		"" :
		join(formatted_arguments)
	);
}

std::string format_instructions (const std::vector<Instruction>& instructions)
{
	std::vector<std::string> formatted;
	std::transform(instructions.begin(), instructions.end(), formatted.begin(), [](const Instruction& instruction){
		std::string formatted_instruction = bool(instruction.descriptor) ?
			format_instruction(instruction) :
			std::string("unknown-command");

		return format_hex_prefixless(instruction.address) + formatted_instruction;
	});

	return join(formatted, "\n");
}
