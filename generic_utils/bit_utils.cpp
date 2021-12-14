//
// Created by Vova on 15.12.2021.
//

#include "bit_utils.h"

u32 get_trailing_ones(usize amount) {
	return (1 << amount) - 1;
}

u32 uint_from_range_sequence (u32 bits, const std::vector<std::pair<usize, usize>>& index_ranges_inclusive)
{
	u32 res{};
	usize res_ptr = 0;
	for (auto& r : index_ranges_inclusive) {
		if (r.second < r.first) {
			continue;
		}
		usize segment_length = r.second + 1 - r.first;
		let segment = (bits >> r.first) & get_trailing_ones(segment_length);

		res |= segment << res_ptr;

		res_ptr += segment_length;
	}

	return res;
}

i32 two_s_complement_int_from_range_sequence (u32 bits, std::vector<std::pair<usize, usize>> index_ranges_inclusive)
{
	usize res_length = 0;
	for (auto s : index_ranges_inclusive) {
		res_length += s.second + 1 - s.first;
	}

	usize sign_bit_index = index_ranges_inclusive.back().second;
	if (index_ranges_inclusive.back().first == index_ranges_inclusive.back().second) {
		index_ranges_inclusive.pop_back();
	} else {
		index_ranges_inclusive.back().second--;
	}

	let res_ptr = res_length - 1;

	return i32(uint_from_range_sequence(bits, index_ranges_inclusive))
				- i32((bits >> sign_bit_index) << res_ptr);
}
