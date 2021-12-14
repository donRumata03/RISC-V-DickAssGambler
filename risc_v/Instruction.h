//
// Created by Vova on 15.12.2021.
//

#pragma once

#include "pch.h"

enum class InstructionSet {
	RV32I,
	RV32M,
	RVC
};



struct OpCode {
	u32 value;
};


struct Instruction {
	u32 address;
	OpCode opcode;


	InstructionSet instruction_set;

//	IntInstructionLayout intLayout;
};


