//
// Created by Vova on 16.12.2021.
//

#pragma once


#include "generic_utils/bit_utils.h"
#include "Instruction.h"


Immediate parse_immediate(u32 command, const RV32InstructionDescriptor& instruction_descriptor);
std::optional<Immediate> maybe_parse_immediate(u32 command, const RV32InstructionDescriptor& instruction_descriptor);
Immediate parse_immediate(u32 command, const std::vector<std::pair<usize, usize>>& ranges, ImmediateSignness signedness);

u32 parse_opcode(u32 command);
u32 parse_excluding_opcode(u32 command);

u32 parse_funct3(u32 command);
u32 parse_funct7(u32 command);

u32 parse_rs1(u32 command);
u32 parse_rs2(u32 command);
u32 parse_rd(u32 command);

u32 parse_csr(u32 command);


Instruction parse_RV32_instruction(u32 command);
