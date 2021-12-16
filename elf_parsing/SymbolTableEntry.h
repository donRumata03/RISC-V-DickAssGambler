//
// Created by Vova on 12.12.2021.
//

#pragma once


enum class SectionTableBind
{
	LOCAL = 0,
	GLOBAL = 1,
	WEAK = 2
};


enum class SectionTableType
{
	NOTYPE = 0,
	OBJECT = 1,
	FUNC = 2,
	SECTION = 3,
	FILE = 4,
	COMMON = 5,
	TLS = 6,
	SPARC_REGISTER = 13,
};


enum class SectionTableVisibility
{
	DEFAULT = 0,
	INTERNAL = 1,
	HIDDEN = 2,
	PROTECTED = 3
};

enum class SectionTableIndex
{
	ABS,
	COMMON,
	XINDEX,
	UNDEF
};

std::ostream& operator << (std::ostream& out, const SectionTableBind& bind);
std::ostream& operator << (std::ostream& out, const SectionTableType& section_table_type);
std::ostream& operator << (std::ostream& out, const SectionTableVisibility& visibility);
std::ostream& operator << (std::ostream& out, const SectionTableIndex& index);




#pragma pack(1)
struct SymbolTableEntry
{
	u32	st_name;
	u32	st_value;
	u32	st_size;
	u8 st_info;
	u8 st_other;
	u16 st_shndx;

	static SymbolTableEntry construct_from_bytes(byte_view bytes);

	[[nodiscard]] std::string get_visibility() const;
	[[nodiscard]] std::string get_bind() const;
	[[nodiscard]] std::string get_type() const;
	[[nodiscard]] std::string get_index() const;
};


