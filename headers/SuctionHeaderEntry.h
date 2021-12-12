//
// Created by Vova on 11.12.2021.
//

#pragma once

#include "pch.h"
#include "generic_utils/bit_utils.h"

enum class SectionHeaderType : u32
{
	SHT_NULL,
	SHT_PROGBITS,
	SHT_SYMTAB,
	SHT_STRTAB
	// Something else…
};
static_assert(sizeof(SectionHeaderType) == sizeof(u32));

#pragma pack(1)
struct SuctionHeaderEntry
{
	u32 name_offset; // An offset to a string in the .shstrtab section that represents the name of this section.
	SectionHeaderType type;
	u32 flags;
	u32 virtual_address; // Address to be loaded (don't need it)
	u32 file_offset; // Offset of the section in the file image.
	u32 size; // Size in bytes of the section in the file image. May be 0.
	u32 link; // Contains the section index of an associated section. This field is used for several purposes, depending on the type of section.
	u32 info;
	u32 alignment;
	u32 entry_size; // Contains the size, in bytes, of each entry, for sections that contain fixed-size entries. Otherwise, this field contains zero.


	static SuctionHeaderEntry construct_from_bytes(byte_view bytes) {
		assert(bytes.size() == sizeof(SuctionHeaderEntry));

		SuctionHeaderEntry res {
			*reinterpret_cast<const SuctionHeaderEntry*>(bytes.data())
		};

		if constexpr(is_big_endian()) {
			reverse_all(
					res.name_offset,
					*reinterpret_cast<u32*>(&res.type),
					res.flags,
					res.virtual_address,
					res.file_offset,
					res.size,
					res.link,
					res.info,
					res.alignment,
					res.entry_size
			);
		}

		return res;
	}
};

static_assert(sizeof(SuctionHeaderEntry) == 0x28);
// static_assert(std::is_integral_v<SectionHeaderType>); <<-- Fails…

