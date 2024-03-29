//
// Created by Vova on 11.12.2021.
//


#include "gtest/gtest.h"
#include <cassert>
#include <generic_utils/file_utils.h>
#include "elf_parsing/ElfHeader.h"
#include "detect_locations.h"


TEST(HeaderTests, FullHeader)
{
	auto header1 = ElfHeader::read_from_bytes(read_bytes(simplest_elf_path));
	auto header2 = ElfHeader::read_from_bytes(read_bytes(big_rv32_elf_path));

	EXPECT_THROW(ElfHeader::read_from_bytes(read_bytes(x64_elf_path)), std::runtime_error);

}
