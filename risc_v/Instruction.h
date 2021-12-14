//
// Created by Vova on 15.12.2021.
//

#pragma once

#include "pch.h"
#include "RV32InstructionDescriptor.h"

enum class InstructionSet {
	RV32,
	RVC
};



struct OpCode {
	u32 value;
};

enum class IntRegister : u32
{
	// € [0, 32)
	zero            = 0,
	ra              = 1,
	sp              = 2,
	gp              = 3,
	tp              = 4,
	t0              = 5,
	t1              = 6,
	t2              = 7,
	fp              = 8,
	s1              = 9,
	a0 = 10, a1 = 11, a2 = 12, a3 = 13, a4 = 14, a5 = 15, a6 = 16, a7 = 17,
	s2 = 18, s3 = 19, s4 = 20, s5 = 21, s6 = 22, s7 = 23, s8 = 24, s9 = 25, s10 = 26, s11 = 27,
	t3 = 28, t4 = 29, t5 = 30, t6 = 31,
};

enum class CsrRegister : u32
{
	// € [0, 2^12)
	fflags = 0x001,
	frm = 0x002,
	fcsr = 0x003,

	cycle = 0xC00,
	time =0xC01,
	instret =0xC02,

	cycleh = 0xC80,
	timeh = 0xC81,
	instreth = 0xC82,
};

using Immediate = std::variant<u32, i32>;

struct Instruction {
	u32 address;

	InstructionSet instruction_set;
	RV32InstructionDescriptor descriptor;

	std::optional<IntRegister> src_register_left;
	std::optional<IntRegister> src_register_right;
	std::optional<IntRegister> dest_register_right;

	std::optional<Immediate> immediate;

	std::optional<CsrRegister> csr_register;


//	IntInstructionLayout intLayout;
};


