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
    enum Direction {
        down = 0,
        left = 1,
        up = 2,
        right = 3,
    };

    struct Cell {
        CellIndex cell_index;
        bool visited;
        uint8_t wall_bits;
        std::vector<CellIndex> adjacent_cells;

        [[nodiscard]] bool HasWall(const Direction dir) const noexcept {
            return (wall_bits & (1u << dir)) != 0;
        }

        [[nodiscard]] bool IsDeadEnd() const noexcept {
            int wall_count = 0;
            if (HasWall(left)) wall_count += 1;
            if (HasWall(right)) wall_count += 1;
            if (HasWall(up)) wall_count += 1;
            if (HasWall(down)) wall_count += 1;
            return wall_count == 3;
        }

        void RemoveWall(const Direction dir) noexcept {
            wall_bits &= ~(1u << dir);
        }
    };


}

template<>
struct std::hash<Gameplay::Mazegenerator::CellIndex> {
    std::size_t operator()(const Gameplay::Mazegenerator::CellIndex &index) const noexcept {
        return (static_cast<std::size_t>(index.x) << 32) ^ index.y;
    }
}; // namespace
