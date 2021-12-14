//
// Created by Vova on 14.12.2021.
//

#pragma once


#include "generic_utils/bit_utils.h"
#include "Instruction.h"


std::vector<Instruction> parseInstructions (const byte_string& text_section, usize start_adderss);

