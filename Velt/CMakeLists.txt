cmake_minimum_required(VERSION 3.20)
project(Velt)

add_library(Velt SHARED "src/Velt/Core/Core.h" "src/Velt/Core/Application.h""src/Velt/Core/Window.h""src/Velt/Core/Application.cpp""src/Velt/Core/Entrypoint.h""src/Velt/Renderer/Renderer.h""src/Velt/Renderer/Renderer.cpp" "src/Velt/Core/Log.h" "src/Velt/Core/Log.cpp" "src/Velt/Platform/Vulkan/VulkanPipeline.h" "src/Velt/Platform/Vulkan/VulkanPipeline.cpp" "src/Velt/Platform/Vulkan/VulkanDevice.cpp" "src/Velt/Platform/Vulkan/VulkanDevice.h" "src/Velt/Platform/Vulkan/VulkanSwapchain.cpp" "src/Velt/Platform/Vulkan/VulkanSwapchain.h" "src/vtpch.cpp" "src/vtpch.h" "src/Velt/Core/Layer.cpp" "src/Velt/Core/Layer.h" "src/Velt/Core/LayerStack.cpp" "src/Velt/Core/LayerStack.h" "src/Velt/ImGui/ImGuiLayer.h" "src/Velt/ImGui/ImGuiLayer.cpp" "src/Velt/ImGui/ImGuiRenderer.h" "src/Velt/ImGui/ImGuiRenderer.cpp" "src/Velt/Core/Instrumentor.h" "src/Velt/Renderer/RenderContext.h" "src/Velt/Renderer/RenderContext.cpp" "src/Velt/Renderer/Pipeline.h" "src/Velt/Renderer/Pipeline.cpp"   "src/Velt/Platform/Vulkan/VulkanRenderer.h"  "src/Velt/Platform/Vulkan/VulkanRenderer.cpp"  "src/Velt/Platform/Vulkan/VulkanContext.h"  "src/Velt/Platform/Vulkan/VulkanContext.cpp"  "src/Velt/Renderer/RenderCommandBuffer.h" "src/Velt/Renderer/RenderCommandBuffer.cpp"  "Velt.h"  "src/Velt/Renderer/Buffer.h" "src/Velt/Renderer/Buffer.cpp" "src/Velt/Platform/Vulkan/Buffer/VulkanVertexBuffer.cpp" "src/Velt/Platform/Vulkan/Buffer/VulkanIndexBuffer.cpp" "src/Velt/Platform/Vulkan/Buffer/VulkanIndexBuffer.h" "src/Velt/Renderer/IndexBuffer.h" "src/Velt/Renderer/IndexBuffer.cpp" "src/Velt/Renderer/VertexBuffer.h" "src/Velt/Renderer/VertexBuffer.cpp" "src/Velt/Platform/Windows/WindowsWindow.h" "src/Velt/Platform/Windows/WindowsWindow.cpp" "src/Velt/Renderer/RenderAPI.h" "src/Velt/Renderer/RenderAPI.cpp" "src/Velt/Platform/Vulkan/VulkanResourceUploader.h" "src/Velt/Platform/Vulkan/VulkanResourceUploader.cpp" "src/Velt/Renderer/SceneRenderer.cpp" 
"src/Velt/Renderer/SceneRenderer.h"      "src/Velt/Renderer/UniformBuffer.h" "src/Velt/Renderer/UniformBuffer.cpp" "src/Velt/Platform/Vulkan/Buffer/VulkanUniformBuffer.h" "src/Velt/Platform/Vulkan/Buffer/VulkanUniformBuffer.cpp" "src/Velt/Platform/Vulkan/DescriptorSetManager.h" "src/Velt/Platform/Vulkan/DescriptorSetManager.cpp" "src/Velt/Core/Timestep.h"  "src/Velt/Core/Input.h"  "src/Velt/Core/Scancodes.h"  "src/Velt/Platform/SDL/SDLInput.h"  "src/Velt/Platform/SDL/SDLInput.cpp"  "src/Velt/Core/Input.cpp" "src/Velt/Events/Event.h" "src/Velt/Events/EventHandler.h" "src/Velt/Events/EventDispatcher.h" "src/Velt/Events/ApplicationEvents.h" "src/Velt/Platform/SDL/SDLEventTranslator.h" "src/Velt/Events/KeyEvents.h" "src/Velt/Events/MouseEvents.h" "src/Velt/Renderer/Model.h"  "src/Velt/Renderer/Model.cpp"  "src/Velt/Platform/Vulkan/DescriptorLayoutCache.cpp" "src/Velt/Platform/Vulkan/DescriptorLayoutCache.h"  "src/Velt/Renderer/Material.h"  "src/Velt/Core/Math.h"  "src/Velt/Renderer/Material.cpp" "src/Velt/Renderer/Texture.h" "src/Velt/Renderer/Texture.cpp"  "src/Velt/Platform/Vulkan/VulkanTexture.h"  "src/Velt/Platform/Vulkan/VulkanTexture.cpp"  "vendor/stb/stb_image.h"  "src/Velt/Renderer/ShaderLibrary.cpp"  "src/Velt/Renderer/ShaderLibrary.h"  "src/Velt/Renderer/PipelineManager.h"  "src/Velt/Renderer/PipelineManager.cpp" "src/Velt/Renderer/Camera.h" "src/Velt/Renderer/Camera.cpp" "src/Velt/Editor/EditorCamera.h" "src/Velt/Editor/EditorCamera.cpp" "src/Velt/Asset/AssimpMeshImporter.h"  "src/Velt/Asset/AssimpMeshImporter.cpp"  "src/Velt/Renderer/DefferedRenderer.h"  "src/Velt/Renderer/DefferedRenderer.cpp" 
"src/Velt/Renderer/BindingLayouts.h" "vendor/entt/single_include/entt/entt.hpp"
 "src/Velt/Scene/Scene.h" "src/Velt/Scene/Scene.cpp" "src/Velt/Scene/Components.h" "src/Velt/Scene/Entity.h"  "src/Velt/Editor/Panels/EditorViewportPanel.h" "src/Velt/Editor/Panels/EditorViewportPanel.cpp" "src/Velt/Editor/EditorGuizmos.h" "src/Velt/Editor/EditorGuizmos.cpp")

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/tracy)


if(APPLE)
    find_package(SDL3 REQUIRED)
else()
    add_subdirectory(vendor/sdl3 EXCLUDE_FROM_ALL)
endif()

find_package(Vulkan REQUIRED)
target_link_libraries(Velt PUBLIC Vulkan::Vulkan)
target_link_libraries(Velt PRIVATE spdlog::spdlog SDL3::SDL3 assimp Tracy::TracyClient)

target_include_directories(Velt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Velt
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl3/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tracy/public/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt/single_include/
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_program(DXC_EXECUTABLE NAMES dxc dxc.exe HINTS "$ENV{VULKAN_SDK}/Bin" REQUIRED)

set(SHADER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shader")
set(SHADER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/CompiledShaders")
file(MAKE_DIRECTORY ${SHADER_BIN_DIR})

set(HLSL_FILES 
    "basic_vertex_shader.hlsl" "vs_6_0"
    "basic_fragment_shader.hlsl" "ps_6_0"
    "gbuffer_vertex.hlsl" "vs_6_0"
    "gbuffer_pixel.hlsl" "ps_6_0"
    "deffered_pbr_vertex.hlsl" "vs_6_0"
    "deffered_pbr_pixel.hlsl" "ps_6_0"
)

set(FORCE_REBUILD "${CMAKE_CURRENT_BINARY_DIR}/FORCE_REBUILD")
add_custom_command(
    OUTPUT ${FORCE_REBUILD}
    COMMAND ${CMAKE_COMMAND} -E echo "Forcing Shader Rebuild"
    VERBATIM
)

set(SPIRV_BINARY_FILES "")
list(LENGTH HLSL_FILES _hlsl_count)
math(EXPR _hlsl_max "${_hlsl_count} - 1")

foreach(_i RANGE 0 ${_hlsl_max} 2)
    list(GET HLSL_FILES ${_i} HLSL_FILE)
    math(EXPR _j "${_i} + 1")
    list(GET HLSL_FILES ${_j} HLSL_PROFILE)
    
    set(SRC_PATH "${SHADER_SRC_DIR}/${HLSL_FILE}")
    set(OUT_PATH "${SHADER_BIN_DIR}/${HLSL_FILE}.spv")
    list(APPEND SPIRV_BINARY_FILES ${OUT_PATH})

    add_custom_command(
        OUTPUT ${OUT_PATH}
        COMMAND ${DXC_EXECUTABLE}
            -T ${HLSL_PROFILE}
            -E main
            -spirv
            -fspv-target-env=vulkan1.3
            "${SRC_PATH}"
            -Fo "${OUT_PATH}"
        DEPENDS "${SRC_PATH}" ${FORCE_REBUILD}
        COMMENT "Force recompiling HLSL: ${HLSL_FILE}"
        VERBATIM
    )
endforeach()

add_custom_target(CompileShaders ALL DEPENDS ${SPIRV_BINARY_FILES})

add_dependencies(Velt CompileShaders)

add_custom_command(
    TARGET Velt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Velt>/Assets/Shader"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/../Assets" "$<TARGET_FILE_DIR:Velt>/Assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SPIRV_BINARY_FILES} "$<TARGET_FILE_DIR:Velt>/Assets/Shader/"
    COMMENT "Deploying Assets and freshly compiled Shaders"
)

target_sources(Velt PUBLIC
    vendor/ImGui/imgui.cpp
    vendor/ImGui/imgui_draw.cpp
    vendor/ImGui/imgui_widgets.cpp
    vendor/ImGui/imgui_tables.cpp
    vendor/ImGui/imgui_demo.cpp
    vendor/ImGui/backends/imgui_impl_sdl3.cpp
    vendor/ImGui/backends/imgui_impl_vulkan.cpp
)

target_compile_definitions(Velt PRIVATE 
    VL_BUILD_DLL
    $<$<CONFIG:Debug>:VL_DEBUG>
    $<$<CONFIG:Release>:VL_RELEASE>
)

target_precompile_headers(Velt PUBLIC src/vtpch.h)

set_target_properties(Velt PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")