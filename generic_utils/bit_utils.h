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



template <typename T, std::enable_if_t<std::is_unsigned_v<T>, void*> literally_nothing = nullptr>
T reverse_byte_order(T number) {
	T res{};

	constexpr std::size_t bytes_T = sizeof(T);

	for (size_t i = 0; i < bytes_T; ++i) {
		res |= ((number >> (i * 8)) & T{0xFFU}) << ((bytes_T - i - 1) * 8);
	}

	return res;
}

