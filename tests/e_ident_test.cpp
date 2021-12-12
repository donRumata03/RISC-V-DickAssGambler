//
// Created by vova on 10.12.2021.
//

#include "gtest/gtest.h"
#include <cassert>
#include <generic_utils/file_utils.h>
#include "elf_parsing/ElfHeaderIdent.h"
#include "detect_locations.h"


TEST(HeaderTests, E_IDENT) {
	auto ident1 = ElfHeaderIdent::read_from_bytes(read_bytes(test1_elf_path));
	auto ident2 = ElfHeaderIdent::read_from_bytes(read_bytes(test2_elf_path));

	EXPECT_THROW(ElfHeaderIdent::read_from_bytes(read_bytes(x64_elf_path)), std::runtime_error);
}

