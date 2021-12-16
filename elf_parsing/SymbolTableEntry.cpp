//
// Created by Vova on 12.12.2021.
//

#include "SymbolTableEntry.h"
#include "generic_utils/bit_utils.h"
#include "generic_utils/formatting_utils.h"


std::ostream& operator<< (std::ostream& out, const SectionTableVisibility& visibility)
{
	switch (visibility) {
		case SectionTableVisibility::DEFAULT:
			out << "DEFAULT";
			break;
		case SectionTableVisibility::INTERNAL:
			out << "INTERNAL";
			break;
		case SectionTableVisibility::HIDDEN:
			out << "HIDDEN";
			break;
		case SectionTableVisibility::PROTECTED:
			out << "PROTECTED";
			break;
	}

	return out;
}

std::ostream& operator<< (std::ostream& out, const SectionTableBind& bind)
{
	switch (bind) {
		case SectionTableBind::LOCAL:
			out << "LOCAL";
			break;
		case SectionTableBind::GLOBAL:
			out << "GLOBAL";
			break;
		case SectionTableBind::WEAK:
			out << "WEAK";
			break;
	}

	return out;
}

#define stt_case(name) case SectionTableType:: name : \
					   out << #name ; \
					   break;

std::ostream& operator<< (std::ostream& out, const SectionTableType& section_table_type)
{
	switch (section_table_type) {
		stt_case(NOTYPE)
		stt_case(OBJECT)
		stt_case(FUNC)
		stt_case(SECTION)
		stt_case(FILE)
		stt_case(COMMON)
		stt_case(TLS)
		stt_case(SPARC_REGISTER)
	}

	return out;
}

std::ostream& operator<< (std::ostream& out, const SectionTableIndex& index)
{
	switch (index) {

		case SectionTableIndex::ABS:
			out << "ABS";
			break;
		case SectionTableIndex::COMMON:
			out << "COMMON";
			break;
		case SectionTableIndex::XINDEX:
			out << "XINDEX";
			break;
		case SectionTableIndex::UNDEF:
			out << "UNDEF";
			break;

		default:
			out << std::to_string(usize(index));
			break;
	}

	return out;
}


SymbolTableEntry SymbolTableEntry::construct_from_bytes (byte_view bytes)
{
	assert(bytes.size() == sizeof(SymbolTableEntry));

	SymbolTableEntry res {
			*reinterpret_cast<const SymbolTableEntry*>(bytes.data())
	};

	if constexpr(is_big_endian()) {
		reverse_all(
				res.st_name,
				res.st_value,
				res.st_size,
				res.st_shndx
		);
	}

	return res;
}

std::string SymbolTableEntry::get_visibility () const
{
	u8 vis_bits = st_other & 0b11;

	return to_string(SectionTableVisibility(vis_bits));
}

std::string SymbolTableEntry::get_bind () const
{
	u8 bind_bits = st_info >> 4;

	return to_string(SectionTableBind(bind_bits));
}


std::string SymbolTableEntry::get_type () const
{
	u8 vis_bits = st_info & 0xf;

	return to_string(SectionTableType(vis_bits));
}

std::string SymbolTableEntry::get_index () const
{
	u8 index_bits = st_shndx;

	return to_string(SectionTableIndex(index_bits));
}
