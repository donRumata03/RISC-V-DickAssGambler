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


TEST(DisAsm, Test1Elf) {
	print_disasm(test1_elf_path);
}

TEST(DisAsm, EcallCommand) {
	let instr = parse_RV32_instruction(0x00000073);

	EXPECT_EQ(format_instruction(instr), "ecall");
}
