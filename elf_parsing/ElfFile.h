//
// Created by Vova on 12.12.2021.
//

#pragma once

#include "pch.h"
#include "ElfHeader.h"
#include "SuctionHeaderEntry.h"
#include "generic_utils/file_utils.h"
#include "ElfSection.h"

class ElfFile
{
public:
	byte_string raw_data;

	ElfHeader header;
	std::vector<SuctionHeaderEntry> section_headers;
	std::vector<ElfSection> sections;


	explicit ElfFile(const fs::path& elf_path) {
		raw_data = read_bytes(elf_path);
		header = ElfHeader::read_from_bytes(byte_view(raw_data));

		read_section_headers();
		read_sections();
	}

	auto get_section_by_name(const std::string& name) -> std::optional<ElfSection> {
		auto lookup_res = std::find_if(sections.begin(), sections.end(), [&name](auto sec){ return sec.name == name; });

		return lookup_res == sections.end() ? std::optional<ElfSection>() : *lookup_res;
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
			sections.push_back(ElfSection {
					.header = section_header,
					.name = "", // We'll get it from one of the sections after parsing them
					.data = raw_data.substr(section_header.file_offset, section_header.size)
			});
		}
		auto section_name_table = sections[header.shstrndx];

		// Get section names:
		for (auto& section : sections) {
			section.name = std::string(reinterpret_cast<const c8*>(section_name_table.data.data()) + section.header.name_offset);
		}
	}
};


