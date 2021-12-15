//
// Created by vova on 10.12.2021.
//


#pragma once

#include "../pch.h"

constexpr const char* detector_header_path = __FILE__;
inline auto disassembler_base_dir = fs::path{detector_header_path}.parent_path().parent_path();
inline auto test1_elf_path = disassembler_base_dir / "tests" / "resources" / "simplest.elf";
inline auto test2_elf_path = disassembler_base_dir / "tests" / "resources" / "big_rv32.elf";
inline auto test1_new_elf_path = disassembler_base_dir / "tests" / "resources" / "rvc.elf";
inline auto x64_elf_path = disassembler_base_dir / "tests" / "resources" / "x64.elf";
