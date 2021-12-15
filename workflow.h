//
// Created by Vova on 17.12.2021.
//

#pragma once

#include "risc_v/command_parser.h"
#include "risc_v/base_instruction_formatter.h"
#include "detect_locations.h"
#include "elf_parsing/ElfFile.h"



inline void print_disasm(const fs::path& filename) {
	auto elf = ElfFile(filename);

	auto text_section = elf.text_section;
	auto symbols = elf.symbols;

	std::cout << format_instructions(parseInstructions(text_section.data, text_section.header.virtual_address)) << std::endl;
}

