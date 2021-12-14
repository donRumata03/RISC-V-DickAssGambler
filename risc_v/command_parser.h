//
// Created by Vova on 14.12.2021.
//

#pragma once


#include "generic_utils/bit_utils.h"

#include "Instruction.h"


Immediate parse_immediate(u32 command, const RV32InstructionDescriptor& instruction_descriptor);
Immediate parse_immediate(u32 command, const std::vector<std::pair<usize, usize>>& ranges, ImmediateSignness signedness);

std::vector<Instruction> parseInstructions ();

