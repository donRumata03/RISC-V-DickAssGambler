//
// Created by Vova on 12.12.2021.
//

#pragma once

#define ELF32_ST_BIND(info)          ((info) >> 4)
#define ELF32_ST_TYPE(info)          ((info) & 0xf)
#define ELF32_ST_INFO(bind, type)    (((bind)<<4)+((type)&0xf))
#define ELF32_ST_VISIBILITY(bind, type)    (((bind)<<4)+((type)&0xf))

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

std::ostream& operator << (std::ostream& out, const SectionTableBind& bind);
std::ostream& operator << (std::ostream& out, const SectionTableType& section_table_type);
std::ostream& operator << (std::ostream& out, const SectionTableVisibility& visibility);




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

	std::string get_visibility() const;
	std::string get_bind() const;
	std::string get_type() const;
};


