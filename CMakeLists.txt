set(headers
    src/espchrono.h
)

set(sources
    src/espchrono.cpp
)

idf_component_register(INCLUDE_DIRS src SRCS ${headers} ${sources} REQUIRES freertos esp_system cpputils date)
