cmake_minimum_required(VERSION 3.17)

set(LINUX_COMPILER_CHOSEN GCC)
#set(LINUX_COMPILER_CHOSEN CLANG)

set(REQUIRED_CLANG_VERSION 12)
set(REQUIRED_GCC_VERSION 10)

if (WIN32)
    message("Platform: Windows")
    message("Note that you should have MSVC compiler if it's Windows")
endif()


if (UNIX)
    message("Platform: *nix")

    if(LINUX_COMPILER_CHOSEN STREQUAL "CLANG")
        message(
        "Note that you should have an LLVM C++ compiler with version at least ${REQUIRED_CLANG_VERSION}: otherwise it won't compile!"
        )
        set(CMAKE_CXX_COMPILER "/usr/bin/clang++-${REQUIRED_CLANG_VERSION}")
        set(CMAKE_C_COMPILER "/usr/bin/clang-${REQUIRED_CLANG_VERSION}")
    endif()
    if(LINUX_COMPILER_CHOSEN STREQUAL "GCC")
        message("Note that you should have a GCC compiler with version ${REQUIRED_GCC_VERSION}: otherwise it won't compile!")
        set(CMAKE_CXX_COMPILER "/usr/bin/g++-${REQUIRED_GCC_VERSION}")
        set(CMAKE_C_COMPILER "/usr/bin/gcc-${REQUIRED_GCC_VERSION}")
    endif()

endif()

message("C++ compiler is set to ${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 20)

project(RISC_V_Disassembler)


include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(RISC_V_Disassembler
        generic_utils/bit_utils.h
        generic_utils/file_utils.h
        generic_utils/string_viewing_utils.h
        generic_utils/formatting_utils.h
        elf_parsing/ElfHeaderIdent.h
        elf_parsing/ElfHeader.h
        elf_parsing/SuctionHeaderEntry.cpp
        elf_parsing/SuctionHeaderEntry.h
        elf_parsing/ElfHeaderIdent.cpp
        elf_parsing/ElfHeader.cpp
        elf_parsing/section_header_reader.cpp
        elf_parsing/section_header_reader.h
        elf_parsing/ElfFile.cpp
        elf_parsing/ElfFile.h
        elf_parsing/ElfSection.cpp
        elf_parsing/ElfSection.h
        elf_parsing/SymbolTableEntry.cpp
        elf_parsing/SymbolTableEntry.h

        elf_parsing/ElfSymbol.cpp elf_parsing/ElfSymbol.h)

add_executable(RISC_V_Disassembler_Runner
        main.cpp
        )

target_precompile_headers(RISC_V_Disassembler PRIVATE pch.h)


add_subdirectory(tests)
