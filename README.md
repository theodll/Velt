# Velt Game Engine

Velt is a 3D game engine powered by the Vulkan API encompassed in a strong RHI. The project architecture is heavily inspired by modern engines such as Hazel, Unity, and the Source Engine, focusing on high-performance rendering and a modular development workflow.

While the primary focus is currently on Vulkan, Velt is designed with an abstraction layer to facilitate the future implementation of other modern graphics APIs. This includes plans for Metal to provide native macOS support and Direct3D 12 for optimized Windows compatibility, though these backends are not planned for the immediate future.

## Showcase
![Sponza Demo](Docs/pic.png)

## Note:
The Forward Pass in this Game Engine is called Scene-Pass (SceneRenderer). 
The Deffered-Pass therefore is called DefferedPass (DefferedRenderer)