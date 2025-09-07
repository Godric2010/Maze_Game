//
// Created by Sebastian Borsch on 07.09.25.
//

#pragma once
#include <vector>

namespace Gameplay::Mazegenerator {
    struct CellIndex {
        uint32_t x;
        uint32_t y;

        bool operator==(const CellIndex &rhs) const noexcept {
            return x == rhs.x && y == rhs.y;
        }
    };


    struct Cell {
        CellIndex cell_index;
        bool visited;
        uint8_t wall_bits;
        std::vector<CellIndex> adjacent_cells;
    };
}

template<>
struct std::hash<Gameplay::Mazegenerator::CellIndex> {
    std::size_t operator()(const Gameplay::Mazegenerator::CellIndex &index) const noexcept {
        return (static_cast<std::size_t>(index.x) << 32) ^ index.y;
    }
}; // namespace
