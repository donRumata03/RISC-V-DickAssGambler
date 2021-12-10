//
// Created by vova on 10.12.2021.
//


#pragma once

#include "pch.h"

std::vector<u8> read_bytes(const fs::path& filename) {
	std::ifstream is(filename);

	is.seekg(0, std::ios_base::end);
	std::size_t file_length = is.tellg();
	is.seekg(0, std::ios_base::beg);

	std::vector<u8> res(file_length);

	is.read(reinterpret_cast<char*>(&res[0]), file_length);
	is.close();

	return res;
}

