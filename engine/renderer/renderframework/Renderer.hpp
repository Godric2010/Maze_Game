#pragma once
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>
#include "../meshmanagement/Mesh.hpp"

namespace Engine::Renderer::RenderFramework {
    /**
     *@class IRenderer
     * Interface to access the renderer and its functions to display an image on the screen
     */
    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        /**
         * Initialize the renderer component
         */
        virtual void Initialize() = 0;

        /**
         * Prepare the frame of the renderer
         */
        virtual void PrepareFrame(glm::mat4 camView, glm::mat4 camProj, glm::vec3 camPos) = 0;

        /**
         * Call to render to the screen
         */
        virtual void DrawFrame() = 0;

       /**
        * Add a mesh to the renderer to display it
        */
        virtual void AddMesh(const Meshmanagement::Mesh& mesh) = 0;

       /**
        * Remove a mesh from the renderer to free its resources.
        */
        virtual void RemoveMesh() = 0;

        /**
         * Disable the renderer and free its resources.
         */
        virtual void Shutdown() = 0;
    };
} // namespace
