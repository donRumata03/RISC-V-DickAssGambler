//
// Created by Vova on 16.12.2021.
//

#pragma once

#include "Instruction.h"


std::string get_int_register_name(IntRegister int_register);
std::string get_csr_register_name(CsrRegister int_register);

std::string format_decimal_immediate(Immediate immediate);


std::string format_instruction(const Instruction& instruction);

std::string format_instructions(const std::vector<Instruction>& instructions);

inline std::ostream& operator<< (std::ostream& os, const CsrRegister& reg)
{
	os << get_csr_register_name(reg);

	return os;
}

inline std::ostream& operator<< (std::ostream& os, const IntRegister& reg)
{
	os << get_int_register_name(reg);

	return os;
}

