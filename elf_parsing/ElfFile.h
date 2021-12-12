//
// Created by Vova on 12.12.2021.
//

#pragma once

#include "pch.h"
#include "ElfHeader.h"
#include "SuctionHeaderEntry.h"
#include "generic_utils/file_utils.h"

class ElfFile
{
	byte_string raw_data;

	ElfHeader header;
	std::vector<SuctionHeaderEntry> section_headers;
	std::vector<byte_string> strings;


	explicit ElfFile(const fs::path& elf_path) {
		raw_data = read_bytes(elf_path);
		header = ElfHeader::read_from_bytes(byte_view(raw_data));

	}
};


