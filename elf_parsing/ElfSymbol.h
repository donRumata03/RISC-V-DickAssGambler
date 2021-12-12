//
// Created by Vova on 12.12.2021.
//

#pragma once

#include "SymbolTableEntry.h"

struct ElfSymbol
{
	SymbolTableEntry entry;
	std::string name;
};


