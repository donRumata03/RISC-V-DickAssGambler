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

template<class T>
std::string join(const std::vector<T>& strings, std::string_view separator = " ") {
	std::string res;

	for (size_t i = 0; i < strings.size(); ++i) {
		if (i != 0) {
			res += separator;
		}
		res += strings[i];
	}

	return res;
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

template <std::integral T>
void dump_bytes(T v, char term = '\n') {
	std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(sizeof(T) * 2) << v << " : ";
	for (std::size_t i{}; i != sizeof(T); ++i, v >>= 8) {
		std::cout << std::setw(2) << static_cast<unsigned>(T(0xFF) & v) << ' ';
	}
	std::cout << std::dec << term;
}


template<std::integral T, size_t fill_size = sizeof(T) * 2>
inline std::string format_hex_prefixless(T v) {
	auto ss = std::stringstream{};

	ss << std::hex << std::uppercase << std::setfill('0') << std::setw(fill_size) << v;

	return ss.str();
}


template<std::integral T>
inline std::string format_hex(T v) {
	return "0x" + format_hex_prefixless(v);
}


inline std::string format_hex_sequence(std::basic_string_view<u8> bytes) {
	std::vector<std::string> strings(bytes.size());
	std::transform(bytes.begin(), bytes.end(), strings.begin(), [](u8 character){
		return format_hex_prefixless<u16, 2>(u16(character));
	});

	return join(strings, " ");
}
