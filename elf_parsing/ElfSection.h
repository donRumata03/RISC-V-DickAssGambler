//
// Created by Vova on 12.12.2021.
//

#pragma once


#include "SuctionHeaderEntry.h"

struct ElfSection
{
	SuctionHeaderEntry header;
	std::string name;
	byte_string data;
};


