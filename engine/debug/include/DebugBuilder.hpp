//
// Created by Sebastian Borsch on 03.12.25.
//

#pragma once
#include <memory>

#include "IDebugConsole.hpp"
#include "../src/DebugConsole.hpp"

namespace yarep::debug
{
    static std::unique_ptr<IDebugConsole> CreateDebugConsole(text::TextController* text,
                                                             renderer::IRenderController* render,
                                                             asset_handling::AssetHandler* asset_handler,
                                                             environment::WindowContext& ctx, uint32_t col_width)
    {
        return std::make_unique<DebugConsole>(text, render, asset_handler, ctx, col_width);
    }
}
