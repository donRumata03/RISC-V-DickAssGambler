//
// Created by vova on 10.12.2021.
//


#pragma once

#include "../pch.h"

constexpr const char* detector_header_path = __FILE__;
inline auto disassembler_base_dir = fs::path{detector_header_path}.parent_path().parent_path();

inline auto resource_path = disassembler_base_dir / "tests" / "resources";

inline auto simplest_dir = resource_path / "simplest";
inline auto simplest_elf_path = simplest_dir / "simplest.elf";
inline auto simplest_answer_path = simplest_dir / "simplest_disassembler_output.txt";
inline auto simplest_testee_path = simplest_dir / "testee_output.txt";

inline auto big_rv32_dir = resource_path / "big_rv32";
inline auto big_rv32_elf_path = big_rv32_dir / "big_rv32.elf";
inline auto big_rv32_answer_path = big_rv32_dir / "big_rv32_disassembler_output.txt";
inline auto big_rv32_testee_path = big_rv32_dir / "testee_output.txt";


inline auto rvc_dir = resource_path / "rvc";
inline auto rvc_elf_path =    rvc_dir / "rvc.elf";
inline auto rvc_answer_path = rvc_dir / "rvc_disassembler_sample.txt";
inline auto rvc_testee_path = rvc_dir / "testee_output.txt";


inline auto x64_elf_path = disassembler_base_dir / "tests" / "resources" / "x64.elf";
