//
// Created by Vova on 12.12.2021.
//

#include "ElfHeaderIdent.h"


bool ElfHeaderIdent::validate_signature ()
{
	return view_char_array<c8>(elf_signature) == EXPECTED_ELF_SIGNATURE;
}

ElfHeaderIdent ElfHeaderIdent::read_from_bytes (byte_view input_bytes)
{
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
				+ format_hex_sequence(byte_view(view_char_array<u8>(res.elf_signature))
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
