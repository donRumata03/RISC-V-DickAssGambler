//
// Created by vova on 08.12.2021.
//

#pragma once

#include "pch.h"

constexpr bool is_big_endian() {
	return std::endian::native == std::endian::big;
}

inline void check_platform_parameters() {
	static_assert(sizeof(char) == 1, "You use such a weird system...");
	static_assert(
			std::endian::native == std::endian::big || std::endian::native == std::endian::little,
			"You should have either big or little endian as native one!"
	);

	if constexpr (std::endian::native == std::endian::big)
		std::cout << "[INFO]: Your native endianness is big-endian. But you could do better...\n";
	else if constexpr (std::endian::native == std::endian::little)
		std::cout << "[INFO]: Your native endianness is little-endian. Brilliant!\n";
}


template <std::unsigned_integral T>
T reverse_byte_order(T number) {
T res{};

constexpr usize bytes_T = sizeof(T);

for (size_t i = 0; i < bytes_T; ++i) {
res |= ((number >> (i * 8)) & T{0xFFU}) << ((bytes_T - i - 1) * 8);
}

return res;
}

template <std::unsigned_integral T>
void reverse_byte_order_in_place(T& number) {
	number = reverse_byte_order(number);
}

template<class ...T>
void reverse_all(T&... args) {
	(reverse_byte_order_in_place(args), ...);
}



u32 get_trailing_ones(usize amount);

/**
 * sign bit is considered to be the last one
 */
i32 two_s_complement_int_from_range_sequence(u32 bits, const std::vector<std::pair<usize, usize>>& index_ranges_inclusive);
