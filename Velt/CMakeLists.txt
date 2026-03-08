add_library(Velt SHARED
        "src/Velt/Core/Core.h"
        "src/Velt/Core/Application.h"
        "src/Velt/Core/Window.h"
        "src/Velt/Core/Application.cpp"
        "src/Velt/Core/Entrypoint.h"
        "src/Velt/Renderer/Renderer.h"
        "src/Velt/Renderer/Renderer.cpp"
        "src/Velt/Core/Log.h"
        "src/Velt/Core/Log.cpp"
        "src/Velt/Platform/Vulkan/VulkanPipeline.h"
        "src/Velt/Platform/Vulkan/VulkanPipeline.cpp"
        "src/Velt/Platform/Vulkan/VulkanDevice.cpp"
        "src/Velt/Platform/Vulkan/VulkanDevice.h"
        "src/Velt/Platform/Vulkan/VulkanSwapchain.cpp"
        "src/Velt/Platform/Vulkan/VulkanSwapchain.h"
        "src/Velt/Renderer/Renderer.h"
        "src/Velt/Renderer/Renderer.cpp"
        "src/vtpch.cpp"
        "src/vtpch.h"
        "src/Velt/Layers/Layer.cpp"
        "src/Velt/Layers/Layer.h"
        "src/Velt/Layers/LayerStack.cpp"
        "src/Velt/Layers/LayerStack.h"
        "src/Velt/ImGui/ImGuiLayer.h"
        "src/Velt/ImGui/ImGuiLayer.cpp"
        "src/Velt/ImGui/ImGuiRenderer.h"
        "src/Velt/ImGui/ImGuiRenderer.cpp"
        "src/Velt/Utils/Profiling/Instrumentor.h"
        "src/Velt/Renderer/RenderContext.h"
        "src/Velt/Renderer/RenderContext.cpp"
        "src/Velt/Renderer/Pipeline.h"
        "src/Velt/Renderer/Pipeline.cpp"  
        "src/Velt/Platform/Vulkan/VulkanRenderer.h" 
        "src/Velt/Platform/Vulkan/VulkanRenderer.cpp" 
        "src/Velt/Platform/Vulkan/VulkanContext.h" 
        "src/Velt/Platform/Vulkan/VulkanContext.cpp" 
        "src/Velt/Renderer/RenderCommandBuffer.h"
        "src/Velt/Renderer/RenderCommandBuffer.cpp" 
        "Velt.h" 
        "src/Velt/Renderer/Buffer.h"
        "src/Velt/Renderer/Buffer.cpp"
        "src/Velt/Platform/Vulkan/Buffer/VulkanVertexBuffer.cpp"
        "src/Velt/Platform/Vulkan/Buffer/VulkanIndexBuffer.cpp"
        "src/Velt/Platform/Vulkan/Buffer/VulkanIndexBuffer.h"
        "src/Velt/Renderer/IndexBuffer.h"
        "src/Velt/Renderer/IndexBuffer.cpp"
        "src/Velt/Renderer/VertexBuffer.h"
        "src/Velt/Renderer/VertexBuffer.cpp"
        "src/Velt/Core/Window.h"
        "src/Velt/Platform/Windows/WindowsWindow.h"
        "src/Velt/Platform/Windows/WindowsWindow.cpp"
        "src/Velt/Renderer/RenderAPI.h"
        "src/Velt/Renderer/RenderAPI.cpp"
        "src/Velt/Platform/Vulkan/VulkanResourceUploader.h"
        "src/Velt/Platform/Vulkan/VulkanResourceUploader.cpp"
        "src/Velt/Renderer/SceneRenderer.cpp" 
        "src/Velt/Renderer/SceneRenderer.h"   
        "src/Velt/ImGui/SceneViewport.h"
        "src/Velt/ImGui/SceneViewport.cpp"
        "src/Velt/Renderer/UniformBuffer.h"
        "src/Velt/Renderer/UniformBuffer.cpp"
        "src/Velt/Platform/Vulkan/Buffer/VulkanUniformBuffer.h"
        "src/Velt/Platform/Vulkan/Buffer/VulkanUniformBuffer.cpp"
        "src/Velt/Platform/Vulkan/DescriptorSetManager.h"
        "src/Velt/Platform/Vulkan/DescriptorSetManager.cpp"
        "src/Velt/Renderer/OrthographicCamera.h"
        "src/Velt/Renderer/OrthographicCamera.cpp"
 "src/Velt/Core/Timestep.h" "src/Velt/Core/Input.h" "src/Velt/Core/Scancodes.h" "src/Velt/Platform/SDL/SDLInput.h" "src/Velt/Platform/SDL/SDLInput.cpp" "src/Velt/Core/Input.cpp"
        src/Velt/Events/Event.h
        src/Velt/Events/EventHandler.h
        src/Velt/Events/EventDispatcher.h
        src/Velt/Events/ApplicationEvents.h
        src/Velt/Platform/SDL/SDLEventTranslator.h
        src/Velt/Events/KeyEvents.h
        src/Velt/Events/MouseEvents.h
  "src/Velt/Renderer/Mesh.cpp" "src/Velt/Renderer/Mesh.h" "src/Velt/Renderer/Model.h" "src/Velt/Renderer/Model.cpp" 
 "src/Velt/Platform/Vulkan/DescriptorLayoutCache.cpp"
"src/Velt/Platform/Vulkan/DescriptorLayoutCache.h" "src/Velt/Renderer/Material.h" "src/Velt/Core/Math.h" "src/Velt/Renderer/Material.cpp"
"src/Velt/Renderer/Texture.h"
 "src/Velt/Renderer/Texture.cpp" "src/Velt/Platform/Vulkan/VulkanTexture.h" "src/Velt/Platform/Vulkan/VulkanTexture.cpp" "vendor/stb_image/stb_image.h" "vendor/stb_image/stb_image.cpp")


if(WIN32 OR UNIX)
    set(SHADER_SRC_DIR ${CMAKE_SOURCE_DIR}/Shaders)
    set(SHADER_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/Shaders)
    file(MAKE_DIRECTORY ${SHADER_OUT_DIR})

    set(SHADER_FILES
        "${SHADER_SRC_DIR}/vertex.glsl"
        "${SHADER_SRC_DIR}/fragment.glsl"
    )

    add_custom_target(
        CompileShaders ALL
    )

    foreach(SHADER_FILE ${SHADER_FILES})
        get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)

        if(SHADER_NAME MATCHES "vertex")
            set(SHADER_STAGE "vertex")
        elseif(SHADER_NAME MATCHES "fragment")
            set(SHADER_STAGE "fragment")
        endif()

        if(WIN32)
            add_custom_command(
                TARGET CompileShaders POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Compiling ${SHADER_NAME}..."
                COMMAND C:/VulkanSDK/1.4.328.1/Bin/glslc.exe -fshader-stage=${SHADER_STAGE} ${SHADER_FILE} -o ${SHADER_OUT_DIR}/${SHADER_NAME}.spv
                VERBATIM
            )
        else()
            add_custom_command(
                TARGET CompileShaders POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Compiling ${SHADER_NAME}..."
                COMMAND glslc -fshader-stage=${SHADER_STAGE} ${SHADER_FILE} -o ${SHADER_OUT_DIR}/${SHADER_NAME}.spv
                VERBATIM
            )
        endif()
    endforeach()

    add_dependencies(Velt CompileShaders)
    target_compile_definitions(Velt PRIVATE VL_BUILD_DLL)
    add_definitions(-DVL_BUILD_DLL)
endif()

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/volk)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/sdl3)

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "Vulkan include dir: ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(Velt PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(Velt PRIVATE ${Vulkan_LIBRARIES} SDL3::SDL3)
endif()

target_include_directories(Velt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl3/include
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Velt PRIVATE
        $<$<CONFIG:Debug>:VL_DEBUG>
        $<$<CONFIG:Release>:VL_RELEASE>
        $<$<CONFIG:Dist>:VL_DIST>
)

target_include_directories(Velt
        PUBLIC
        ${CMAKE_SOURCE_DIR}/Velt/vendor/spdlog/include/
        ${CMAKE_SOURCE_DIR}/Velt/vendor/glfw/include/
        ${CMAKE_SOURCE_DIR}/Velt/vendor/glm/
        ${CMAKE_SOURCE_DIR}/Velt/vendor/stb_image/
        ${CMAKE_SOURCE_DIR}/Velt/src/
        ${CMAKE_SOURCE_DIR}/Velt/src/Velt
)

target_precompile_headers(Velt PUBLIC ${CMAKE_SOURCE_DIR}/Velt/src/vtpch.h)

message(STATUS "VL_BUILD_DLL is defined: ${VL_BUILD_DLL}")
get_target_property(COMPILE_DEFS Velt COMPILE_DEFINITIONS)
message(STATUS "Velt compile definitions: ${COMPILE_DEFS}")

if(WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(IMGUI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/backends/imgui_impl_sdl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/backends/imgui_impl_vulkan.cpp
)

target_sources(Velt PRIVATE ${IMGUI_SRC})

target_include_directories(Velt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui/backends
)

set_target_properties(Velt PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)