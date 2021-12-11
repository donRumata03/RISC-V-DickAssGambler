//
// Created by vova on 11.12.2021.
//


#pragma once

#include "pch.h"

template <typename C>
concept Character =
		std::is_same_v<C, signed char> ||
        std::is_same_v<C, char> ||
        std::is_same_v<C, unsigned char>;


template <Character To, Character From>
inline auto view_as(std::basic_string_view<From> target) {
	return std::basic_string_view<To>(reinterpret_cast<const To*>(target.data()), target.size());
}


/**
 * @param target shouldn't change: otherwise, output may become invalid
 */
template <Character To, Character From>
inline auto view_as(const std::basic_string<From>& target) {
	return std::basic_string_view<To>(reinterpret_cast<const To*>(target.data()), target.size());
}


template <Character CharTo, Character CharFrom>
inline auto view_chars(const CharFrom* target, size_t length) {
	return std::basic_string_view<CharTo>(
			reinterpret_cast<const CharTo*>(target), length
			);
}

template <Character CharTo, size_t array_length, Character CharFrom>
inline auto view_char_array(const CharFrom (&target)[array_length]) {
	return std::basic_string_view<CharTo>(reinterpret_cast<const CharTo*>(std::data(target)), array_length);
}

template<Character CharTo, std::integral T>
auto view_as(T& integer) {
	return view_chars<CharTo>(reinterpret_cast<CharTo*>(&integer), sizeof(T));
}
