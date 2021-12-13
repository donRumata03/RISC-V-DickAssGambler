//
// Created by Vova on 15.12.2021.
//

#include "bit_utils.h"

u32 get_trailing_ones(usize amount) {
	return (1 << amount) - 1;
}

i32 two_s_complement_int_from_range_sequence (u32 bits, const std::vector<std::pair<usize, usize>>& index_ranges_inclusive)
{
	usize res_length = 0;
	for (auto term : index_ranges_inclusive | rv::transform([](auto v){ return v.second - v.first + 1; })) {
		res_length += term;
	}

	u32 res{};
	usize res_ptr = 0;
	for (auto& r : index_ranges_inclusive) {
		usize segment_length = r.second + 1 - r.first;
		let segment = (bits >> r.first) & get_trailing_ones(segment_length);

		res |= segment << res_ptr;

		res_ptr += segment_length;
	}

	assert(res_length == res_ptr);

	return i32(res) - i32(bits & (1 << (res_length - 1)));
}
