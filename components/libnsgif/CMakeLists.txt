idf_component_register(
    SRCS
        libnsgif/src/libnsgif.c
        libnsgif/src/lzw.c
    INCLUDE_DIRS
        libnsgif/include
    PRIV_INCLUDE_DIRS 
        .
        libnsgif/src
)

set_source_files_properties(libnsgif/src/lzw.c PROPERTIES COMPILE_FLAGS -Wno-maybe-uninitialized)

