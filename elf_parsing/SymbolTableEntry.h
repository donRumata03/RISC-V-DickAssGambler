//
// Created by Vova on 12.12.2021.
//

#pragma once


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
};


