//
// Created by Vova on 11.12.2021.
//

#include "SuctionHeaderEntry.h"


SuctionHeaderEntry SuctionHeaderEntry::construct_from_bytes (byte_view bytes)
{
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
