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

TEST(DisAsm, SImmediate) {
	//	04f18c23          	sb	a5,88(gp)
	let sb_88 = parse_RV32_instruction(0x04f18c23);

	// 11964:	fee78fa3          	sb	a4,-1(a5)
	let sb_minus_1 = parse_RV32_instruction(0x04f18c23);


	EXPECT_EQ(std::get<i32>(*sb_88.immediate), 88);
}
