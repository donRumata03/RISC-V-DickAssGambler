//
// Created by vova on 11.12.2021.
//


#pragma once

#include "pch.h"

#include "string_viewing_utils.h"

template<typename Left, typename Right, typename ResultType = Left>
concept AddableToAs = requires (Left l, Right r) { {l + r} -> std::convertible_to<ResultType>; };

template<class T> requires AddableToAs<std::string, T>
std::string join(const std::vector<T>& strings, std::string_view separator = " ");

template <std::integral T> requires (!Character<T>)
void dump_bytes(T v, char term = '\n');

template<std::integral T, size_t fill_size = sizeof(T) * 2> requires (!Character<T>)
std::string format_hex_prefixless(T v);

std::string format_hex_sequence(std::basic_string_view<u8> bytes);


/// Implementations

template <class T>
std::string join (const std::vector<T>& strings, std::string_view separator)
{
	std::string res;

	for (size_t i = 0; i < strings.size(); ++i) {
		if (i != 0) {
			res += separator;
		}
		res += strings[i];
	}

	return res;
}

template <std::integral T> requires (!Character<T>)
void dump_bytes (T v, char term)
{
	std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(sizeof(T) * 2) << v << " : ";
	for (std::size_t i{}; i != sizeof(T); ++i, v >>= 8) {
		std::cout << std::setw(2) << static_cast<unsigned>(T(0xFF) & v) << ' ';
	}
	std::cout << std::dec << term;
}


template <std::integral T, size_t fill_size> requires (!Character<T>)
std::string format_hex_prefixless (T v)
{
	auto ss = std::stringstream{};

	ss << std::hex << std::uppercase << std::setfill('0') << std::setw(fill_size) << v;

	return ss.str();
}


template <std::integral T> requires (Character<T>)
std::string format_hex_prefixless (T v)
{
	return format_hex_prefixless<u16, 2>(u16(u8(v)))
}

template<std::integral T>
std::string format_hex(T v) {
	return "0x" + format_hex_prefixless(v);
}


