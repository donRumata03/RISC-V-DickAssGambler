//
// Created by Vova on 12.12.2021.
//


#include "ElfHeader.h"


ElfHeader ElfHeader::read_from_bytes (byte_view input_bytes)
{
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

//		assert(res.phentsize == 32);
	assert(res.shentsize == 40);

	return res;
}

