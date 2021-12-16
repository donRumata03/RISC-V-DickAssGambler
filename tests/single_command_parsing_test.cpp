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



TEST(DisAsm, EcallCommand) {
	let instr = parse_RV32_instruction(0x00000073);

	EXPECT_EQ(format_instruction(instr), "ecall");
}


TEST(SingleCommand, BImmediate) {
/**
 * <b>0_000000_00000_01111_000_1000_0</b>1100011 --> beq	a5, zero, 10088
 *
 * Immediate: 0|0|000000|1000|0 ->> 16
 */
	let instr_forward = parse_RV32_instruction(0x00078863);

	//      fa0688e3          	beq	a3, zero, 103c0 <__call_exitprocs+0x68>
	// Immediate = -80
	let instr_backward = parse_RV32_instruction(0xfa0688e3);

	EXPECT_EQ(std::get<i32>(*instr_forward.immediate), 16);
	EXPECT_EQ(std::get<i32>(*instr_backward.immediate), -80);
}

TEST(SingleCommand, JImmediate) {
/**
 * It's only:
 * imm[20|10:1|11|19:12] rd 1101111 JAL
 */

	//	0600006f          	jal	zero,104e8 <__register_exitproc>
	// Immediate = 96
	let instr_forward = parse_RV32_instruction(0x0600006f);

	//	f5dff06f          	jal	zero,103a0 <__call_exitprocs+0x48>
	// Immediate = -164
	let instr_backward = parse_RV32_instruction(0xf5dff06f);

	EXPECT_EQ(std::get<i32>(*instr_forward.immediate), 96);
	EXPECT_EQ(std::get<i32>(*instr_backward.immediate), -164);
}

TEST(SingleCommand, IImmediate) {
	//    00000793          	addi	a5,zero,0
	ASSERT_EQ(0x00000793, 1939);
	let instr_zero = parse_RV32_instruction(0x00000793);


	//    4fc50513          	addi	a0, a0, 1276
	let instr_pos = parse_RV32_instruction(0x4fc50513);

	//    dfc18193          	addi	gp, gp, -516
	let instr_neg = parse_RV32_instruction(0xdfc18193);

	EXPECT_EQ(std::get<i32>(*instr_pos.immediate), 1276);
	EXPECT_EQ(std::get<i32>(*instr_zero.immediate), 0);
	EXPECT_EQ(std::get<i32>(*instr_neg.immediate), -516);
}

TEST(SingleCommand, SImmediate) {
	//	04f18c23          	sb	a5,88(gp)
	let sb_88 = parse_RV32_instruction(0x04f18c23);

	// fee78fa3          	sb	a4,-1(a5)
	let sb_minus_1 = parse_RV32_instruction(0xfee78fa3);


	EXPECT_EQ(std::get<i32>(*sb_88.immediate), 88);
	EXPECT_EQ(std::get<i32>(*sb_minus_1.immediate), -1);
}

TEST(SingleCommand, AdressImmediates) {
	let beq = parse_RV32_instruction(493667);

	EXPECT_TRUE(beq.immediate);
	EXPECT_TRUE(std::holds_alternative<i32>(*beq.immediate));
}

TEST(SingleCommand, InstructionCopyConstructor) {
	// 00000793          	addi	a5,zero,0
	let sample_instr = parse_RV32_instruction(0x00000793);

	std::vector<Instruction> instructions;
	instructions.push_back(sample_instr);
	instructions.push_back(sample_instr);

	EXPECT_EQ(sample_instr, instructions[0]);
	EXPECT_EQ(sample_instr, instructions[1]);
}


TEST(SingleCommand, InstructionMoveConstructor) {
	// 00000793          	addi	a5,zero,0
	let sample_instr = parse_RV32_instruction(0x00000793);

	Instruction copied = sample_instr;
	Instruction new_instruction = std::move(sample_instr);

	EXPECT_EQ(copied, new_instruction);
}


TEST(SingleCommand, InstructionMoveInVector) {
	// 00000793          	addi	a5,zero,0
	let sample_instr = parse_RV32_instruction(0x00000793);
	let copy = sample_instr;
	EXPECT_EQ(sample_instr, copy);

	std::vector<Instruction> vec;
	vec.push_back(sample_instr);

	std::vector<Instruction> move_dest = std::move(vec);

	EXPECT_EQ(move_dest[0], sample_instr);
}


std::vector<Instruction> gen_vector(const Instruction& instruction) {
	std::vector<Instruction> res;
	res.push_back(instruction);

	return res;
}

TEST(SingleCommand, InstructionMoveInVectorAtFunctionReturn) {
	let sample_instr = parse_RV32_instruction(493667);
	let copy = sample_instr;
	EXPECT_EQ(sample_instr, copy);

	EXPECT_FALSE(sample_instr.immediate->valueless_by_exception());
	EXPECT_FALSE(copy.immediate->valueless_by_exception());

	std::vector<Instruction> vec = gen_vector(sample_instr);

	EXPECT_EQ(vec[0], sample_instr);
	EXPECT_FALSE(vec[0].immediate->valueless_by_exception());

}

