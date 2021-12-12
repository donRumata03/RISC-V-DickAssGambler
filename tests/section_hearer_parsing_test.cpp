//
// Created by Vova on 12.12.2021.
//

#include "gtest/gtest.h"
#include <cassert>
#include <generic_utils/file_utils.h>
#include "detect_locations.h"
#include "elf_parsing/SuctionHeaderEntry.h"
#include "elf_parsing/ElfFile.h"


TEST(SuctionHeaderTests, FullSuctionHeader)
{
	auto elf1 = ElfFile(test1_elf_path);
	auto elf2 = ElfFile(test2_elf_path);

	EXPECT_THROW(ElfFile{x64_elf_path}, std::runtime_error);
}
