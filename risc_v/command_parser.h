//
// Created by Vova on 14.12.2021.
//

#pragma once

enum class InstructionSet {
	RV32I,
	RV32M,
	RVC
};

enum class IntInstructionLayout {
	R,
	I,
	S,
	SB,
	U,
	UJ,
};

struct Instruction {
	InstructionSet instruction_set;

	IntInstructionLayout intLayout;

};


std::vector<InstructionType> ();

