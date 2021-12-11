//
// Created by vova on 10.12.2021.
//


#pragma once

#include <bit>
#include <cstdint>
#include <concepts>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <type_traits>
#include <vector>


#include <filesystem>
namespace fs = std::filesystem;

using u8 = std::uint8_t;
using i8 = std::int8_t;
using c8 = char;

using u16 = std::uint16_t;
using i16 = std::int16_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

using u64 = std::uint64_t;
using i64 = std::int64_t;


using bytes_view = std::basic_string_view<u8>;
