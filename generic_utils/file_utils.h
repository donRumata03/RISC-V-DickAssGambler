//
// Created by vova on 10.12.2021.
//


#pragma once

#include "pch.h"

inline byte_string read_bytes(const fs::path& filename) {
	std::ifstream is(filename, std::ios::in | std::ios::binary);
	if (!is.is_open()) {
		throw std::runtime_error("Can't open file: " + filename.string());
	}

	is.seekg(0, std::ios_base::end);
	usize file_length = is.tellg();
	is.seekg(0, std::ios_base::beg);

	byte_string res(file_length, '\0');

	is.read(reinterpret_cast<char*>(&res[0]), isize(file_length));
	is.close();

	return res;
}

