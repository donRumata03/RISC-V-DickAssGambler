//
// Created by vova on 08.12.2021.
//

#pragma once
#include "pch.h"
#include "generic_utils/bit_utils.h"
#include "generic_utils/string_viewing_utils.h"
#include "generic_utils/formatting_utils.h"


enum class ElfHeaderEndianness : unsigned char
{
	LITTLE_ENDIAN_ELF = 1,
	BIG_ENDIAN_ELF = 2
};

enum class ElfHeaderClass : unsigned char {
	x32 = 1,
	x64 = 2
};


#pragma pack(1)
struct ElfHeaderIdent
{
	char elf_signature[4];
	ElfHeaderClass elf_class;
	ElfHeaderEndianness endianness;
	u8 version;

	constexpr static std::string_view EXPECTED_ELF_SIGNATURE = "\x7f\x45\x4c\x46";

	bool validate_signature() {
		return view_char_array<c8>(elf_signature) == EXPECTED_ELF_SIGNATURE;
	}

	static ElfHeaderIdent read_from_bytes(const std::vector<u8>& input_bytes) {
		if (input_bytes.size() < sizeof(ElfHeaderIdent)) {
			throw std::runtime_error("Too short file: can't read the ELF header");
		}

		auto reference = reinterpret_cast<const ElfHeaderIdent*>(&input_bytes[0]);
		ElfHeaderIdent res {*reference};

		if (!res.validate_signature()) {
			throw std::runtime_error(
					"File signature doesn't match the ELF one. Expected: "
					+ format_hex_sequence(view_as<u8>(EXPECTED_ELF_SIGNATURE))
					+ "; Actual: "
					+ format_hex_sequence(bytes_view(view_char_array<u8>(res.elf_signature))
					));
		}

		switch (res.endianness) {
			case ElfHeaderEndianness::BIG_ENDIAN_ELF:
				throw std::runtime_error("Can't work with big endian ELF files");
			case ElfHeaderEndianness::LITTLE_ENDIAN_ELF:
				break;
			default:
				throw std::runtime_error(
					"Unknown endianness byte: " + format_hex(u8(res.endianness))
				);
		}

		switch (res.elf_class) {
			case ElfHeaderClass::x64:
				throw std::runtime_error("Sorry, but only 64-bit ELFs are currently supported");
			case ElfHeaderClass::x32:
				break;
			default:
				throw std::runtime_error("Bitness byte: " + format_hex(u8(res.elf_class)));
		}

		return res;
	}
};

