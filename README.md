# RISC-V disassembler

A small disassembler for risc-v architecture.

Currently, these command sets are supported:
- RV32-I
- RV32-M
- RVC


> Please note that `.idea/*`, `tests/*` and `*/CmakeLists.txt` files are not rubbish and should lie in the repository!

## Building

To build the project, you need one of the latest versions 
of one of the three major C++ compilers (GCC-10, MSVC 143+, CLang-12).

Moreover, specifically MSVC is required for Windows 
(for Unix you should choose one of GCC or CLang in the beginning of ./CMakeLists.txt).


