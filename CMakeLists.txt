set(headers
    src/espchrono.h
)

set(sources
    src/espchrono.cpp
    src/espchrono_impl.cpp
)

set(dependencies
    freertos
    esp_timer

    cpputils
    date
    expected
    fmt
)

idf_component_register(
    INCLUDE_DIRS
        src
    SRCS
        ${headers}
        ${sources}
    REQUIRES
        ${dependencies}
)

target_compile_options(${COMPONENT_TARGET}
    PRIVATE
        -fstack-reuse=all
        -fstack-protector-all
        -Wno-unused-function
        -Wno-deprecated-declarations
        -Wno-missing-field-initializers
        -Wno-parentheses
)
