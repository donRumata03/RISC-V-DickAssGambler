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

	std::string format_symtab() {
		std::vector<std::string> formatted_lines;

		formatted_lines.push_back(
				string_format(
						"%s %-15s %7s %-8s %-8s %-8s %6s %s",
						"Symbol", "Value", "Size", "Type", "Bind", "Vis", "Index", "Name"
			)
		);

		usize index = 0;
		for (const auto& symbol: symbols) {
			formatted_lines.push_back(
					string_format(
							"[%4i] 0x%-15X %5i %-8s %-8s %-8s %6s %s",
							index,
							symbol.entry.st_value,
							symbol.entry.st_size,
							symbol.entry.get_type().c_str(),
							symbol.entry.get_bind().c_str(),
							symbol.entry.get_visibility().c_str(),
							"(((I)))",
							symbol.name.c_str()
					)
			);
			index++;
		}

		return join(formatted_lines, "\n");
	}

private:
	void read_section_headers();

	void read_sections();

	void parse_text();

	void parse_symtab();
};


