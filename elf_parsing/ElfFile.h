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
	std::unordered_map<u32, ElfSymbol> symbol_by_address;

	ElfSection text_section;


	explicit ElfFile(const fs::path& elf_path);

	auto get_section_by_name(const std::string& name) -> std::optional<ElfSection>;

	[[nodiscard]] std::optional<ElfSymbol> get_symbol_by_address(u32 address) const {
		auto search_res = symbol_by_address.find(address);

		return search_res != symbol_by_address.end() ?
			std::optional(search_res->second) :
			   std::nullopt;
	}

private:
	void read_section_headers();

	void read_sections();

	void parse_text();

	void parse_symtab();
};


