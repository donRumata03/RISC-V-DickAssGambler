//
// Created by Vova on 12.12.2021.
//

#include "ElfFile.h"

ElfFile::ElfFile (const fs::path& elf_path)
{
	raw_data = read_bytes(elf_path);
	header = ElfHeader::read_from_bytes(byte_view(raw_data));

	read_section_headers();
	read_sections();

	parse_text();
	parse_symtab();
}

auto ElfFile::get_section_by_name (const std::string& name) -> std::optional<ElfSection>
{
	auto lookup_res = std::find_if(sections.begin(), sections.end(), [&name](auto sec){ return sec.name == name; });

	return lookup_res == sections.end() ? std::optional<ElfSection>() : *lookup_res;
}

void ElfFile::read_section_headers ()
{
	for (usize entry_start = header.shoff; entry_start < header.shoff + header.shentsize * header.shnum; entry_start += header.shentsize) {
		auto this_view = view_slice(raw_data, entry_start, header.shentsize);
		section_headers.push_back(SuctionHeaderEntry::construct_from_bytes(this_view));
	}
}

void ElfFile::read_sections ()
{
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

void ElfFile::parse_text ()
{
	auto text_search = get_section_by_name(".text");
	if (!text_search) {
		throw std::runtime_error("Your elf must contain .text section in order to be disassembled");
	} else {
		text_section = *text_search;
	}
}

void ElfFile::parse_symtab ()
{
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
	if (symtab_section.header.link == 0) {
		throw std::runtime_error("SymTab section header must have link to string table with symbol names");
	}
	ElfSection symbol_name_table = sections[symtab_section.header.link];

	for (SymbolTableEntry& symbol_table_entry : symbol_table_entries) {
		auto symbol = ElfSymbol {
				.entry = symbol_table_entry,
				.name = std::string(
						reinterpret_cast<const c8 *>(symbol_name_table.data.data()) + symbol_table_entry.st_name)
		};

		symbols.push_back(symbol);
		symbol_by_address[symbol_table_entry.st_value] = symbol;
	}
}
