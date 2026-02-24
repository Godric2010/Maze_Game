//
// Created by Sebastian Borsch on 03.12.25.
//

#pragma once
#include <memory>

#include "IDebugConsole.hpp"
#include "../src/DebugConsole.hpp"

namespace Engine::Debug
{
    static std::unique_ptr<IDebugConsole> CreateDebugConsole(Text::TextController* text,
                                                             Renderer::IRenderController* render,
                                                             AssetHandling::AssetHandler* asset_handler,
                                                             Environment::WindowContext& ctx, uint32_t col_width)
    {
        return std::make_unique<DebugConsole>(text, render, asset_handler, ctx, col_width);
    }
}
