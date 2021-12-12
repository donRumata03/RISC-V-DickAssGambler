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

	auto symtab_1 = *elf1.get_section_by_name(".symtab");
	auto text_1 = *elf1.get_section_by_name(".text");

	auto symtab_2 = *elf2.get_section_by_name(".symtab");
	auto text_2 = *elf2.get_section_by_name(".text");


	EXPECT_THROW(ElfFile{x64_elf_path}, std::runtime_error);
}
