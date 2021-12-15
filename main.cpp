#include "pch.h"
#include "generic_utils/bit_utils.h"
#include "tests/detect_locations.h"

#include "risc_v/command_parser.h"
#include "risc_v/base_instruction_formatter.h"
#include "elf_parsing/ElfFile.h"


void print_disasm(const fs::path& filename) {
	auto elf = ElfFile(filename);

	auto text_section = elf.text_section;
	auto symbols = elf.symbols;

	std::cout << format_instructions(parseInstructions(text_section.data, text_section.header.virtual_address)) << std::endl;
}

int main(int argv, char** argc)
{
	if (argv != 2) {
		std::cout << "Usage: \n\t<binary-name> <path>" << std::endl;
	}

	check_platform_parameters();

	print_disasm({argc[1]});

//	test_reversing();

	return 0;
}