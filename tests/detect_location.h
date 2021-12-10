//
// Created by vova on 10.12.2021.
//


#pragma once

#include "../pch.h"

constexpr const char* detector_header_path = __FILE__;
inline auto disassembler_base_dir = fs::path{detector_header_path}.parent_path().parent_path();
