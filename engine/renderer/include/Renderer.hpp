#pragma once

namespace Engine::Renderer {
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
         * Call to render to the screen
         */
        virtual void Render() = 0;

        /**
         * Disable the renderer and free its resources.
         */
        virtual void Shutdown() = 0;
    };
} // namespace
