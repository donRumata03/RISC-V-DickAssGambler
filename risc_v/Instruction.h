//
// Created by Vova on 15.12.2021.
//

#pragma once

#include <ostream>
#include "pch.h"
#include "RV32InstructionDescriptor.h"
//#include "base_instruction_formatter.h"

enum class InstructionSet {
	RV32,
	RVC


};

inline std::ostream& operator<< (std::ostream& os, const InstructionSet& instruction_set)
{
	os << ((instruction_set == InstructionSet::RVC) ? "RVC" : "RV32");

	return os;
}



enum class IntRegister : u32
{
	// € [0, 32)
	zero            = 0,
	ra              = 1,
	sp              = 2,
	gp              = 3,
	tp              = 4,
	t0              = 5,
	t1              = 6,
	t2              = 7,
	fp              = 8,
	s1              = 9,
	a0 = 10, a1 = 11, a2 = 12, a3 = 13, a4 = 14, a5 = 15, a6 = 16, a7 = 17,
	s2 = 18, s3 = 19, s4 = 20, s5 = 21, s6 = 22, s7 = 23, s8 = 24, s9 = 25, s10 = 26, s11 = 27,
	t3 = 28, t4 = 29, t5 = 30, t6 = 31,
};

enum class CsrRegister : u32
{
	// € [0, 2^12)
	fflags = 0x001,
	frm = 0x002,
	fcsr = 0x003,

	cycle = 0xC00,
	time =0xC01,
	instret =0xC02,

	cycleh = 0xC80,
	timeh = 0xC81,
	instreth = 0xC82,
};

using Immediate = std::variant<u32, i32>;
Immediate shifted_left(Immediate immediate, usize amount);

inline std::ostream& operator<< (std::ostream& os, const Immediate& imm)
{
	if (imm.valueless_by_exception()) {
		os << "OMG: valueless_by_exception: OMG";
	}

	if (std::holds_alternative<i32>(imm)) {
		os << "i32: " << std::get<i32>(imm);
	}
	else if (std::holds_alternative<u32>(imm)) {
		os << "u32: " << std::get<u32>(imm);
	}
	return os;
}


template<class T> std::ostream& operator<< (std::ostream& os, const std::optional<T>& opt) {
	if (opt) {
		os << "Some(";
		os << *opt;
		os << ")";
	} else {
		os << "None";
	}

	return os;
}

template<class T> std::ostream& operator<< (std::ostream& os, const std::vector<T>& vec) {
	os << "[";
	size_t i = 0;
	for (const auto& el : vec) {
		os << el;

		if (i != vec.size() - 1) os << ",\n";
		i++;
	}
	os << "]";

	return os;
}



struct Instruction {
	u32 address = 0;

	InstructionSet instruction_set{};
	std::optional<RV32InstructionDescriptor> descriptor{};

	std::optional<IntRegister> src_register_left{};
	std::optional<IntRegister> src_register_right{};
	std::optional<IntRegister> dest_register{};

	std::optional<Immediate> immediate{};

	std::optional<CsrRegister> csr_register{};

	[[nodiscard]] std::optional<u32> maybe_get_jmp_address() const
	{
		if (!descriptor.has_value()) {
			return {};
		}

		if (descriptor->contains_static_address_offset) {
			assert(immediate);
			auto real_imm = *immediate;
			auto i = real_imm.index();
			auto i1 = immediate->index();
			assert(!immediate->valueless_by_exception());
			assert(!real_imm.valueless_by_exception());
			assert(std::holds_alternative<i32>(real_imm));
			return u32(address + std::get<i32>(real_imm));
		}

		return {};
	}

	bool operator== (const Instruction& rhs) const
	{
		return address == rhs.address &&
		       instruction_set == rhs.instruction_set &&
		       descriptor == rhs.descriptor &&
		       src_register_left == rhs.src_register_left &&
		       src_register_right == rhs.src_register_right &&
		       dest_register == rhs.dest_register &&
		       immediate == rhs.immediate &&
		       csr_register == rhs.csr_register;
	}

	bool operator!= (const Instruction& rhs) const
	{
		return !(rhs == *this);
	}


	friend std::ostream& operator<< (std::ostream& os, const Instruction& instruction);
};




