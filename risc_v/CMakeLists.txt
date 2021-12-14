add_library(RISC_V_Disassembler
        risc_v_disassembler.h
        risc_v_disassembler.cpp
        command_parser.cpp command_parser.h Instruction.cpp Instruction.h InstructionArgument.cpp InstructionArgument.h RV32InstructionDescriptor.h base_instruction_formatter.cpp base_instruction_formatter.h rv32_parser.cpp rv32_parser.h rvc_parser.cpp rvc_parser.h)
target_precompile_headers(RISC_V_Disassembler PRIVATE ../pch.h)
target_link_libraries(RISC_V_Disassembler GenericUtils ElfParsing)