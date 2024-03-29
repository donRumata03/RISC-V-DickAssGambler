//
// Created by vova on 08.12.2021.
//

#pragma once
#include "pch.h"
#include "generic_utils/bit_utils.h"
#include "generic_utils/string_viewing_utils.h"
#include "generic_utils/formatting_utils.h"


enum class ElfHeaderEndianness : u8
{
	LITTLE_ENDIAN_ELF = 1,
	BIG_ENDIAN_ELF = 2
};

enum class ElfHeaderClass : u8 {
	x32 = 1,
	x64 = 2
};

// OS ABI identification (from LLVM library)
enum class ElfOsABI : u8 {
	SYSTEM_V = 0,       // UNIX System V ABI
	HPUX = 1,           // HP-UX operating system
	NETBSD = 2,         // NetBSD
	GNU = 3,            // GNU/Linux
	LINUX = 3,          // Historical alias for ELFOSABI_GNU.
	HURD = 4,           // GNU/Hurd
	SOLARIS = 6,        // Solaris
	AIX = 7,            // AIX
	IRIX = 8,           // IRIX
	FREEBSD = 9,        // FreeBSD
	TRU64 = 10,         // TRU64 UNIX
	MODESTO = 11,       // Novell Modesto
	OPENBSD = 12,       // OpenBSD
	OPENVMS = 13,       // OpenVMS
	NSK = 14,           // Hewlett-Packard Non-Stop Kernel
	AROS = 15,          // AROS
	FENIX_OS = 16,       // FenixOS
	CLOUDABI = 17,      // Nuxi CloudABI
	FIRST_ARCH = 64,    // First architecture-specific OS ABI
	AMDGPU_HSA = 64,    // AMD HSA runtime
	AMDGPU_PAL = 65,    // AMD PAL runtime
	AMDGPU_MESA3D = 66, // AMD GCN GPUs (GFX6+) for MESA runtime
	ARM = 97,           // ARM
	C6000_ELFABI = 64,  // Bare-metal TMS320C6000
	C6000_LINUX = 65,   // Linux TMS320C6000
	STANDALONE = 255,   // Standalone (embedded) application
	LAST_ARCH = 255     // Last Architecture-specific OS ABI
};



#pragma pack(1)
struct ElfHeaderIdent
{
	u8 elf_signature[4];
	ElfHeaderClass elf_class;
	ElfHeaderEndianness endianness;
	u8 version;
	ElfOsABI os_abi;
	u8 abi_version;
	u8 padding[7];

	constexpr static std::string_view EXPECTED_ELF_SIGNATURE = "\x7f\x45\x4c\x46";



	bool validate_signature();
	static ElfHeaderIdent read_from_bytes(byte_view input_bytes);
};

static_assert(sizeof(ElfHeaderIdent) == 16);

