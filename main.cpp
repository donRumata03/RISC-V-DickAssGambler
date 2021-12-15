#include "pch.h"
#include "generic_utils/bit_utils.h"
#include "tests/detect_locations.h"

#include "risc_v/command_parser.h"
#include "risc_v/base_instruction_formatter.h"
#include "elf_parsing/ElfFile.h"

int main(int argv, char** argc)
{
	if (argv != 2) {
		std::cout << "Usage: \n\t<binary-name> <path>" << std::endl;
		return -1;
	}

	check_platform_parameters();

	print_disasm({argc[1]});

//	test_reversing();

	return 0;
}