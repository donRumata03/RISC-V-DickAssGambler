# RISC-V disassembler

A small disassembler for risc-v architecture.

Currently, these command sets are supported:
- RV32-I
- RV32-M
- ~RVC~


> Please note that `.idea/*`, `tests/*` and `*/CmakeLists.txt` files are not rubbish and should lie in the repository!

## Building

To build the project, you need one of the latest versions 
of one of the three major C++ compilers (tested with: GCC-10, GCC-11, MSVC 143+, CLang-12).

Moreover, specifically MSVC is required for Windows 
(for Unix you should choose one of GCC or CLang in the beginning of ./CMakeLists.txt by uncommenting one of these two lines:
```
set(LINUX_COMPILER_CHOSEN GCC)
# set(LINUX_COMPILER_CHOSEN CLANG)
```
).

The last step is to get the googleTest library by simple `git clone`
and to build && run

```
# Assume you're in repo root

cd tests
git clone https://github.com/google/googletest lib
```

Then - just run CMake as usual. For example, on linux it looks like this:
```
cd ../
mkdir cmake-build-debug
cd cmake-build-debug/
cmake -DCMAKE_BUILD_TYPE=Debug ../
make
./RISC_V_Disassembler_Runner <binary-name> <output-path>  # Name of executable (on Windows it would be prefixed by .exe)

```

If you prefer Release mode, make a separate directory for its build files `cmake-build-release`.
Obviously, the cmake invocation turns into: `cmake -DCMAKE_BUILD_TYPE=Release ../`
