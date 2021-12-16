//
// Created by Vova on 11.12.2021.
//


#include "gtest/gtest.h"
#include <cassert>
#include <generic_utils/file_utils.h>
#include "elf_parsing/ElfFile.h"
#include "detect_locations.h"


TEST(ElfFile, ReadFile) {
	ElfFile file(simplest_elf_path);
	ElfFile file2 = file;

//	ASSERT_EQ(file, file2);

}
