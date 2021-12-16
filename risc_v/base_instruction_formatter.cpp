//
// Created by Vova on 16.12.2021.
//

#include <generic_utils/formatting_utils.h>
#include "base_instruction_formatter.h"

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

std::string format_instructions (const std::vector<Instruction>& instructions)
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


std::string format_labeled_instructions (const std::vector<Instruction>& instructions)
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
