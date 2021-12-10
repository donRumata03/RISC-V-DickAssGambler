//
// Created by vova on 08.12.2021.
//


#pragma once


#include "gtest/gtest.h"
#include <cassert>
#include "../bit_utils.h"

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