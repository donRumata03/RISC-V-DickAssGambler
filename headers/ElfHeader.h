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

	u16 type;
	u16 machine;   // Should be 0xF3 for RISC-V
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
	u16 shstrndx;  // Contains index of the section header table entry that contains the section names.


	static ElfHeader read_from_bytes(byte_view input_bytes);
};

static_assert(sizeof(ElfHeader) == 52);