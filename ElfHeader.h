//
// Created by vova on 11.12.2021.
//


#pragma once

#include "pch.h"

#include "ElfHeaderIdent.h"
#include "generic_utils/formatting_utils.h"


#pragma pack(1)
struct ElfHeader {
	ElfHeaderIdent ident;

	u16 type;      // Type of file (see ET_* below)
	u16 machine;   // Required architecture for this file (see EM_*)
	u32 version;   // Must be equal to 1
	u32 entry;     // Address to jump to in order to start program
	u32 phoff;     // Program header table's file offset, in bytes
	u32 shoff;     // Section header table's file offset, in bytes
	u32 flags;     // Processor-specific flags
	u16 ehsize;    // Size of ELF header, in bytes
	u16 phentsize; // Size of an entry in the program header table
	u16 phnum;     // Number of entries in the program header table
	u16 shentsize; // Size of an entry in the section header table
	u16 shnum;     // Number of entries in the section header table
	u16 shstrndx;  // Sect hdr table index of sect name string table


	static ElfHeader read_from_bytes(byte_view input_bytes) {
		auto _ident = ElfHeaderIdent::read_from_bytes(input_bytes); // May throw exceptions

		ElfHeader res {
			*reinterpret_cast<const ElfHeader*>(input_bytes.data())
		};

		if constexpr(is_big_endian()) {
			reverse_all(
					res.type,
					res.machine,
					res.version,
					res.entry,
					res.phoff,
					res.shoff,
					res.flags,
					res.ehsize,
					res.phentsize,
					res.phnum,
					res.shentsize,
					res.shnum,
					res.shstrndx
			);
		}

		if (res.version != 1) {
			throw std::runtime_error("„Version” should be equal to 1. Actual: " + format_hex_sequence(view_as<u8>(res.version)));
		}

		if (res.machine != 0xF3) {
			throw std::runtime_error("Only RISC-V is supported");
		}
		assert(res.ehsize == 52);

		return res;
	}
};

static_assert(sizeof(ElfHeader) == 52);