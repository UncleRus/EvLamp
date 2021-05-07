idf_component_register(
    SRCS
        mustach.c
        mustach-wrap.c
        mustach-cjson.c
    INCLUDE_DIRS
        .
    REQUIRES
        json
)

