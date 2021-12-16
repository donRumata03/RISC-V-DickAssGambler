//
// Created by Vova on 17.12.2021.
//

#pragma once

#include "risc_v/command_parser.h"
#include "risc_v/base_instruction_formatter.h"
#include "risc_v/LabeledProgram.h"
#include "elf_parsing/ElfFile.h"



inline std::string do_primitive_disasm(const fs::path& filename) {
	auto elf = ElfFile(filename);

	auto text_section = elf.text_section;
	auto symbols = elf.symbols;

	return ".text\n" +
		format_instructions(elf, parseInstructions(text_section.data, text_section.header.virtual_address))
		+ "\n\n.symtab\n"
		+ elf.format_symtab()
		;
}

inline std::string do_labeled_disasm(const fs::path& filename) {
	auto program = LabeledProgram(filename);

	return program.render_program();
}

inline void print_disasm(const fs::path& filename) {
	std::cout << do_primitive_disasm(filename) << std::endl;
}

inline void disasm_to_file_and_console(const fs::path& elf_path, const fs::path& output_path) {
	std::string res = do_primitive_disasm(elf_path);
	std::cout << res << std::endl;

	write_file(output_path, res);
}
