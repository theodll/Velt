add_library(Lavendel SHARED
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Core.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Application.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Application.cpp"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Entrypoint.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Renderer/Renderer.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Renderer/Renderer.cpp"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Log.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Log.cpp"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Events/ApplicationEvent.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Events/Event.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Events/KeyEvent.h"
        "${CMAKE_SOURCE_DIR}/Lavendel/src/Lavendel/Events/MouseEvent.h"
 "src/Lavendel/Renderer/Window.cpp" "src/Lavendel/Renderer/Window.h" "src/Lavendel/Renderer/Pipeline/Pipeline.h" 
"src/Lavendel/Renderer/Pipeline/Pipeline.cpp" "src/Lavendel/Renderer/Core/Device.cpp" "src/Lavendel/Renderer/Core/Device.h"
"src/Lavendel/Renderer/Core/Swapchain.cpp" "src/Lavendel/Renderer/Core/Swapchain.h" "src/Lavendel/Renderer/Renderer.h" "src/Lavendel/Renderer/Renderer.cpp"
        src/Lavendel/Renderer/Model/Model.cpp
        src/Lavendel/Renderer/Model/Model.h)

if(WIN32)
    target_compile_definitions(Lavendel PRIVATE LV_BUILD_DLL)
    add_definitions(-DLV_BUILD_DLL)
endif()

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/volk)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/sdl3)


find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "Vulkan include dir: ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(Lavendel PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(Lavendel PRIVATE ${Vulkan_LIBRARIES}
                                   SDL3::SDL3)
endif()

target_include_directories(Lavendel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_compile_definitions(Lavendel PRIVATE
        $<$<CONFIG:Debug>:LV_DEBUG>
        $<$<CONFIG:Release>:LV_RELEASE>
        $<$<CONFIG:Dist>:LV_DIST>
)


target_include_directories(Lavendel
        PUBLIC
        ${CMAKE_SOURCE_DIR}/Lavendel/vendor/spdlog/include/
        ${CMAKE_SOURCE_DIR}/Lavendel/vendor/glfw/include/
        ${CMAKE_SOURCE_DIR}/Lavendel/vendor/glm/
        ${CMAKE_SOURCE_DIR}/Lavendel/src/
)

target_link_libraries(Lavendel PRIVATE glfw)

message(STATUS "LV_BUILD_DLL is defined: ${LV_BUILD_DLL}")
get_target_property(COMPILE_DEFS Lavendel COMPILE_DEFINITIONS)
message(STATUS "Lavendel compile definitions: ${COMPILE_DEFS}")

if(WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()