//
// Created by vova on 10.12.2021.
//

#include "gtest/gtest.h"
#include <cassert>
#include <file_utils.h>
#include "../ElfHeaderIdent.h"
#include "detect_locations.h"


TEST(HeaderTests, E_IDENT) {
	std::cout << test1_elf_path << std::endl;
	std::cout << test2_elf_path << std::endl;

	auto ident1 = ElfHeaderIdent::read_from_bytes(read_bytes(test1_elf_path));
	auto ident2 = ElfHeaderIdent::read_from_bytes(read_bytes(test2_elf_path));
//	auto ident_failure = ElfHeaderIdent::read_from_bytes(read_bytes(x64_elf_path));


}

