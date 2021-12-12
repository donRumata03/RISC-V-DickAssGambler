//
// Created by Vova on 12.12.2021.
//

#pragma once

#include "pch.h"
#include "ElfHeader.h"
#include "SuctionHeaderEntry.h"
#include "generic_utils/file_utils.h"
#include "ElfSection.h"
#include "SymbolTableEntry.h"
#include "ElfSymbol.h"

class ElfFile
{
public:
	byte_string raw_data;

	ElfHeader header;
	std::vector<SuctionHeaderEntry> section_headers;
	std::vector<ElfSection> sections;

	std::vector<SymbolTableEntry> symbol_table_entries;
	std::vector<ElfSymbol> symbols;

	ElfSection text_section;


	explicit ElfFile(const fs::path& elf_path) {
		raw_data = read_bytes(elf_path);
		header = ElfHeader::read_from_bytes(byte_view(raw_data));

		read_section_headers();
		read_sections();

		parse_text();
		parse_symtab();
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

	void parse_text() {
		auto text_search = get_section_by_name(".text");
		if (!text_search) {
			throw std::runtime_error("Your elf must contain .text section in order to be disassembled");
		} else {
			text_section = *text_search;
		}
	}

	void parse_symtab() {
		auto symtab_search = get_section_by_name(".symtab");

		if (!symtab_search) {
			throw std::runtime_error("Your elf must contain .symtab section in order to be disassembled");
		}

		// Firstly — get entries:
		auto symtab_section = *symtab_search;


		usize entry_size = symtab_section.header.entry_size;
		usize file_offset = symtab_section.header.file_offset;
		usize header_size = symtab_section.header.size;
		for (
				usize entry_start = file_offset;
				entry_start < file_offset + header_size;
				entry_start += entry_size
			)
		{
			auto this_view = view_slice(raw_data, entry_start, entry_size);
			symbol_table_entries.push_back(SymbolTableEntry::construct_from_bytes(this_view));
		}

		// Get symbol names => put them into ElfSymbols together with entries:

	}
};


