add_library(GenericUtils
        bit_utils.h
        file_utils.h
        string_viewing_utils.h
        formatting_utils.h
        generic_utils.cpp bit_utils.cpp coroutine_support/future.h coroutine_support/generator.h)
target_precompile_headers(GenericUtils PRIVATE ../pch.h)

set_target_properties(GenericUtils PROPERTIES LINKER_LANGUAGE CXX)
