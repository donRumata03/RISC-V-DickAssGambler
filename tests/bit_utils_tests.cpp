//
// Created by vova on 08.12.2021.
//


#pragma once


#include "gtest/gtest.h"
#include <cassert>
#include <generic_utils/formatting_utils.h>
#include "generic_utils/bit_utils.h"
#include "risc_v/Instruction.h"

TEST(BitUtils, TestReversing)
{
	EXPECT_EQ(
			reverse_byte_order(static_cast<unsigned char>('a')), static_cast<unsigned char>('a')
	);

	std::cout << "byteswap for U16:\n";
	constexpr auto x = std::uint16_t(0xCAFE);
	dump_bytes(x);
	dump_bytes(reverse_byte_order(x));
	EXPECT_EQ(
			reverse_byte_order(x), 0xFECA
	);

	std::cout << "\nbyteswap for U32:\n";
	constexpr auto y = std::uint32_t(0xDEADBEEFu);
	dump_bytes(y);
	dump_bytes(reverse_byte_order(y));
	EXPECT_EQ(
			reverse_byte_order(y), 0xEFBEADDEu
	);

	std::cout << "\nbyteswap for U64:\n";
	constexpr auto z = std::uint64_t { 0x0123456789ABCDEFull };
	dump_bytes(z);
	dump_bytes(reverse_byte_order(z));
	EXPECT_EQ(
			reverse_byte_order(z), 0xEFCDAB8967452301ull
	);
}

TEST(BitUtils, TestAllReversing)
{
	auto a = u8{0xFEu};
	auto b = u16(0xCAFE);
	auto c = u32(0xDEADBEEFu);
	auto d = u64 { 0x0123456789ABCDEFull };

	reverse_all(a, b, c, d);

	EXPECT_EQ(a, u8{0xFEu});
	EXPECT_EQ(b, u16{0xFECA});
	EXPECT_EQ(c, u32{0xEFBEADDEu});
	EXPECT_EQ(d, u64{0xEFCDAB8967452301ull});
}


TEST(BitUtils, TestConstructingFromBitSequence) {
	u32 bit_sequence = 0b01001100011u;

	auto test = [&](const std::vector<std::pair<usize, usize>>& segments){ return two_s_complement_int_from_range_sequence(bit_sequence, segments); };

	EXPECT_EQ(test({ { 0, 1 }, { 4, 4 }, { 6, 6 }, { 9, 10 } }), 0b011011);
	EXPECT_EQ(test({ { 0, 1 }, { 6, 6 }, { 9, 9 } }), -1);
}

TEST(BitUtils, ShiftingVariant) {
	EXPECT_EQ(std::get<u32>(shifted_left({16u}, 12)), 10088u);
	EXPECT_EQ(std::get<i32>(shifted_left({1}, 12)), 2);
}