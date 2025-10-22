
file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_library(Lavendel SHARED ${ENGINE_SOURCES})


target_include_directories(Lavendel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(Lavendel PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

target_compile_definitions(Lavendel PRIVATE
        $<$<CONFIG:Debug>:LV_DEBUG>
        $<$<CONFIG:Release>:LV_RELEASE>
        $<$<CONFIG:Dist>:LV_DIST>
)

if (WIN32)
    target_compile_definitions(Lavendel PRIVATE LV_BUILD_DLL)
endif ()

target_include_directories(Lavendel
        PRIVATE
        ${CMAKE_SOURCE_DIR}Lavendel/vendor/spdlog/include/
        ${CMAKE_SOURCE_DIR}Lavendel/src/
)