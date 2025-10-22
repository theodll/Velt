
file(GLOB_RECURSE SANDBOX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(Sandbox ${SANDBOX_SOURCES})


target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Lavendel/src
    ${CMAKE_SOURCE_DIR}/Lavendel/vendor
        ${CMAKE_SOURCE_DIR}/vendor/spdlog/include/
        ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(Sandbox PRIVATE Lavendel)

set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)
