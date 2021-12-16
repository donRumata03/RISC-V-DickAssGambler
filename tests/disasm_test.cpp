//
// Created by Vova on 16.12.2021.
//

#include "gtest/gtest.h"


#include "risc_v/command_parser.h"
#include "risc_v/base_instruction_formatter.h"
#include "detect_locations.h"
#include "elf_parsing/ElfFile.h"
#include "workflow.h"
#include "risc_v/rv32_parser.h"
#include "risc_v/InstructionArgument.h"



TEST(DisAsm, SimplestElf) {
	disasm_to_file_and_console(simplest_elf_path, simplest_testee_path);
}

TEST(DisAsm, BigRV32Elf) {
	disasm_to_file_and_console(big_rv32_elf_path, big_rv32_testee_path);
}

TEST(DisAsm, RVCElf) {
	disasm_to_file_and_console(rvc_elf_path, rvc_testee_path);
}


