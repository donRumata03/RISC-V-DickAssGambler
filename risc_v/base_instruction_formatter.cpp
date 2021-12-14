//
// Created by Vova on 16.12.2021.
//

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