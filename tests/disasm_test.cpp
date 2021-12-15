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


TEST(DisAsm, Test1Elf) {
	print_disasm(test1_elf_path);
}

TEST(DisAsm, Test2Elf) {
	print_disasm(test2_elf_path);
}

TEST(DisAsm, EcallCommand) {
	let instr = parse_RV32_instruction(0x00000073);

	EXPECT_EQ(format_instruction(instr), "ecall");
}

TEST(DisAsm, BImmediate) {
	// 1111000100001100011 --> beq	a5, zero, 10088
	let instr = parse_RV32_instruction(0x00078863);

	EXPECT_EQ(std::get<i32>(*instr.immediate), 10088);
}

TEST(DisAsm, IImmediate) {
	//    4fc50513          	addi	a0, a0, 1276
	let instr_pos = parse_RV32_instruction(0x4fc50513);

	//    dfc18193          	addi	gp, gp, -516
	let instr_neg = parse_RV32_instruction(0xdfc18193);

	EXPECT_EQ(std::get<i32>(*instr_pos.immediate), 1276);
	EXPECT_EQ(std::get<i32>(*instr_neg.immediate), -516);
}
