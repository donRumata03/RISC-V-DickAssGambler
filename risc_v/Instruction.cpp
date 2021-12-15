//
// Created by Vova on 15.12.2021.
//

#include "Instruction.h"

Immediate shifted_left (Immediate immediate, usize amount)
{
	if (std::holds_alternative<u32>(immediate)) {
		return std::get<u32>(immediate) << amount;
	}

	return std::get<i32>(immediate) << amount;
}
