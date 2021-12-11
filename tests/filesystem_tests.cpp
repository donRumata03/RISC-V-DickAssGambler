//
// Created by vova on 10.12.2021.
//

#include "gtest/gtest.h"
#include <cassert>
#include "generic_utils/file_utils.h"
#include "detect_locations.h"

TEST(FileSystem, ReadBytes) {
	auto test_path = disassembler_base_dir / "tests" / "resources" / "test_text.txt";
	std::cout << "Reading file: " << test_path << "\n";
	auto bytes = read_bytes(test_path);
	std::string s (bytes.begin(), bytes.end());
	std::cout << s << "\n";
}