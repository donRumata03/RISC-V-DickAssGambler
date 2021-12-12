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
public:
	byte_string raw_data;

	ElfHeader header;
	std::vector<SuctionHeaderEntry> section_headers;
	std::vector<byte_string> sections;


	explicit ElfFile(const fs::path& elf_path) {
		raw_data = read_bytes(elf_path);
		header = ElfHeader::read_from_bytes(byte_view(raw_data));

		read_section_headers();

	}

private:
	void read_section_headers() {
		for (usize entry_start = header.shoff; entry_start < header.shoff + header.shentsize * header.shnum; entry_start += header.shentsize) {
			auto this_view = view_slice(raw_data, entry_start, header.shentsize);
			section_headers.push_back(SuctionHeaderEntry::construct_from_bytes(this_view));
		}
	}

	void read_sections() {
		for (auto& section_header : section_headers) {
//			sections.push_back()
		}
	}
};


