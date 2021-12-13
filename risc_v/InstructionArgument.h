//
// Created by Vova on 15.12.2021.
//

#pragma once

struct RegisterIndex {
	u32 index;
};



using InstructionArgument = std::variant<RegisterIndex>;


