//
// Created by vova on 10.12.2021.
//

#include "gtest/gtest.h"
#include <cassert>
#include "../file_utils.h"
#include "detect_location.h"

TEST(FileSystem, ReadBytes) {
	auto test_path = disassembler_base_dir / "tests" / "test_text.txt";
	std::cout << "Reading file: " << test_path << "\n";
	auto bytes = read_bytes(disassembler_base_dir);
	std::string s (bytes.begin(), bytes.end());
	std::cout << s << "\n";
}