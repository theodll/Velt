# ImGui Rendering Data Flow

## Frame Execution Flow

```
Application::Run()
?
?? Each Frame Loop Iteration:
?  ?
?  ?? m_Window.PollEvents()
?  ?  ?? Processes window events, input
?  ?
?  ?? m_Renderer->beginImGuiFrame()  ? START IMGUI FRAME
?  ?  ?
?  ?  ?? ImGui_ImplVulkan_NewFrame()
?  ?  ?? ImGui_ImplSDL3_NewFrame()
?  ?  ?? ImGui::NewFrame()
?  ?     ?? ImGui context ready for commands
?  ?
?  ?? for (Layer* layer : m_LayerStack)
?  ?  ?   layer->OnUpdate()
?  ?  ?
?  ?  ?? ImGuiLayer::OnUpdate()
?  ?     ?
?  ?     ?? m_DemoWidget.OnRender()
?  ?        ?
?  ?        ?? ImGui::Begin("Lavendel Demo")
?  ?           ?? ImGui::Text(...)
?  ?           ?? ImGui::SliderFloat(...)
?  ?           ?? ImGui::InputInt(...)
?  ?           ?? ImGui::Checkbox(...)
?  ?           ?? ImGui::Button(...)
?  ?           ?? ImGui::End()
?  ?              ?? Widget geometry queued in ImGui draw list
?  ?
?  ?? m_Renderer->endImGuiFrame()  ? END IMGUI FRAME
?  ?  ?? ImGui::Render()
?  ?     ?? Generates ImDrawData (vertex/index buffers)
?  ?
?  ?? m_Renderer->drawFrame()  ? RENDER TO SCREEN
?     ?
?     ?? m_SwapChain->acquireNextImage(&imageIndex)
?     ?  ?? Get next image from swap chain
?     ?
?     ?? recordCommandBuffer(imageIndex)
?     ?  ?
?     ?  ?? vkBeginCommandBuffer()
?     ?  ?
?     ?  ?? vkCmdBeginRenderPass()
?     ?  ?  ?? Start render pass with render pass object
?     ?  ?
?     ?  ?? [Scene Rendering]
?     ?  ?  ?? m_Pipeline->bind(commandBuffer)
?     ?  ?  ?? m_Model->bind(commandBuffer)
?     ?  ?  ?? m_Model->draw(commandBuffer)
?     ?  ?     ?? Geometry rendered to render pass
?     ?  ?
?     ?  ?? [ImGui Rendering] ? KEY STEP
?     ?  ?  ?? m_ImGuiRenderer.Render(commandBuffer)
?     ?  ?     ?? ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer)
?     ?  ?        ?? ImGui vertex/index buffers submitted
?     ?  ?           ?? UI rendered ON TOP of scene
?     ?  ?
?     ?  ?? vkCmdEndRenderPass()
?     ?  ?
?     ?  ?? vkEndCommandBuffer()
?     ?
?     ?? m_SwapChain->submitCommandBuffers(...)
?     ?  ?? Queue command buffer for GPU execution
?     ?
?     ?? m_SwapChain->presentImage(...)
?        ?? Display rendered image on screen
?
?? Repeat for next frame
```

## Memory/Data Structures Involved

### ImGui State
```
ImGui Context
?? Input state (mouse, keyboard)
?? Style information
?? Font atlas
?? Draw list (commands to render)
```

### ImGui Draw Data (after End())
```
ImDrawData
?? Display size (screen dimensions)
?? Frame buffer scale
?? DrawLists (one per window)
    ?? DrawList
        ?? Vertex buffer (positions + colors)
        ?? Index buffer (triangle indices)
        ?? DrawCmd (one per texture)
```

### Vulkan Command Buffer Recording
```
VkCommandBuffer
?? VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO
?? Scene Commands
?  ?? vkCmdBindPipeline
?  ?? vkCmdSetViewport
?  ?? vkCmdSetScissor
?  ?? vkCmdBindVertexBuffers
?  ?? vkCmdBindIndexBuffer
?  ?? vkCmdDrawIndexed
?
?? ImGui Commands
   ?? ImGui vertex buffer binding
   ?? ImGui index buffer binding
   ?? ImGui descriptors (textures)
   ?? vkCmdDrawIndexed (for each ImGui draw command)
```

## Key Integration Points

### 1. **Initialization (Constructor)**
```
Renderer::Renderer()
?? Create GPUDevice (Vulkan context)
?? Create SwapChain (render targets)
?? Create CommandBuffers (GPU command recording)
?? initImGui()
   ?? m_ImGuiRenderer.Init(instance, physicalDevice, device, queue, ...)
      ?? Create descriptor pool
      ?? Initialize ImGui_ImplVulkan
      ?? ImGui context ready
```

### 2. **Frame Execution**
```
beginImGuiFrame()
?? ImGui_ImplVulkan_NewFrame()    ? Handles Vulkan-specific setup
?? ImGui_ImplSDL3_NewFrame()      ? Handles input from SDL3
?? ImGui::NewFrame()              ? Start ImGui frame

[Layer::OnUpdate() ? ImGui commands queued]

endImGuiFrame()
?? ImGui::Render()                ? Generate draw data

drawFrame()
?? recordCommandBuffer()
   ?? ImGui_ImplVulkan_RenderDrawData()  ? Submit to GPU
```

### 3. **Rendering Pipeline**
```
Render Pass
?? Attachment 0: Color image
?? Attachment 1: Depth image
?? Subpass 0:
   ?? Scene render: Uses custom pipeline
   ?? ImGui render: Uses ImGui pipeline
   ?? Both write to same color attachment
```

## Why This Works

1. **Same Render Pass**: Both scene and ImGui render within the same `vkCmdBeginRenderPass()` / `vkCmdEndRenderPass()` block
2. **Rendering Order**: ImGui commands submitted AFTER scene commands ? rendered on top
3. **Proper Descriptor Management**: ImGui maintains its own descriptor pool for texture management
4. **Efficient**: Single render pass minimizes GPU overhead

## Performance Characteristics

| Aspect | Impact |
|--------|--------|
| **Draw Calls** | One per ImGui window (typically 1-5) |
| **Memory Overhead** | Descriptor pool ~10MB |
| **CPU Time** | ImGui: ~0.1-1ms per frame |
| **GPU Time** | Depends on UI complexity, usually <1ms |
| **Latency** | No additional frame latency |

## Debugging Tips

### To verify ImGui is rendering:
1. Run the application
2. Look for "Lavendel Demo" window on screen
3. Check if widgets respond to mouse/keyboard input
4. Toggle "Show ImGui Demo Window" to see more features

### Common Issues & Solutions:

| Issue | Cause | Solution |
|-------|-------|----------|
| ImGui not visible | Not calling `beginImGuiFrame()` | Check Application::Run() |
| ImGui hidden by scene | Rendering order wrong | Ensure ImGui renders after scene |
| ImGui freeze | Input not being processed | Check event handling in ImGuiLayer |
| Crashes on init | Missing Vulkan objects | Ensure initImGui() after SwapChain |

## Future Enhancements

- [ ] Implement mouse/keyboard event handling
- [ ] Add ImGui docking support
- [ ] Multiple viewports
- [ ] Custom fonts loading
- [ ] ImGui state serialization (save/load UI layouts)
