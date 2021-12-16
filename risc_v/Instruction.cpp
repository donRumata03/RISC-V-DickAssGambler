//
// Created by Vova on 15.12.2021.
//

#include "Instruction.h"

#include "base_instruction_formatter.h"


Immediate shifted_left (Immediate immediate, usize amount)
{
	if (std::holds_alternative<u32>(immediate)) {
		return u32(std::get<u32>(immediate) << amount);
	}

	return i32(std::get<i32>(immediate) << amount);
}


std::ostream& operator<< (std::ostream& os, const RV32InstructionDescriptor& descriptor)
{
	os << "name: " << descriptor.name << " pattern: " << "…" << " opcode: " << descriptor.opcode
	   << " funct3: " << descriptor.funct3 << " funct7: " << descriptor.funct7 << " immediate_signedness: "
	   << descriptor.immediate_signedness << " match_after_opcode: " << descriptor.match_after_opcode
	   << " contains_static_address_offset: " << descriptor.contains_static_address_offset;
	return os;
}


std::ostream& operator<< (std::ostream& os, const Instruction& instruction)
{
	os << "address: " << instruction.address << " instruction_set: " << instruction.instruction_set
	   << " descriptor: " << instruction.descriptor << " src_register_left: " << instruction.src_register_left
	   << " src_register_right: " << instruction.src_register_right << " dest_register: "
	   << instruction.dest_register << " immediate: " << instruction.immediate << " csr_register: "
	   << instruction.csr_register;
	return os;
}
