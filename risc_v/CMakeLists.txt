add_library(RISC_V_Disassembler
        risc_v_disassembler.h
        risc_v_disassembler.cpp
        command_parser.cpp command_parser.h Instruction.cpp Instruction.h InstructionArgument.cpp InstructionArgument.h)
target_precompile_headers(RISC_V_Disassembler PRIVATE ../pch.h)
target_link_libraries(RISC_V_Disassembler GenericUtils ElfParsing)