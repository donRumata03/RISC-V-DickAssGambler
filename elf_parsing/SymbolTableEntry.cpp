//
// Created by Vova on 12.12.2021.
//

#include "SymbolTableEntry.h"
#include "generic_utils/bit_utils.h"


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


