COMPONENT_ADD_INCLUDEDIRS := libnsgif/include
COMPONENT_PRIV_INCLUDEDIRS := . libnsgif/src
COMPONENT_SRCDIRS := libnsgif/src

libnsgif/src/lzw.o: CFLAGS += -Wno-maybe-uninitialized
