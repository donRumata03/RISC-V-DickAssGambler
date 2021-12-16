add_library(ElfParsing
        ElfHeaderIdent.h
        ElfHeader.h
        SuctionHeaderEntry.cpp
        SuctionHeaderEntry.h
        ElfHeaderIdent.cpp
        ElfHeader.cpp
        section_header_reader.cpp
        section_header_reader.h
        ElfFile.cpp
        ElfFile.h
        ElfSection.cpp
        ElfSection.h
        SymbolTableEntry.cpp
        SymbolTableEntry.h
        ElfSymbol.cpp
        ElfSymbol.h
        ../risc_v/LabeledProgram.cpp ../risc_v/LabeledProgram.h)
target_precompile_headers(ElfParsing PRIVATE ../pch.h)
target_link_libraries(ElfParsing GenericUtils)
