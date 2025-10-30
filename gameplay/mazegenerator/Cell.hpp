//
// Created by Sebastian Borsch on 07.09.25.
//

#pragma once
#include <vector>

namespace Gameplay::Mazegenerator {
    /**
     * @struct CellIndex
     * @brief Represents a utility struct to manage and work with the index of a cell
     *        in a grid or matrix structure.
     */
    struct CellIndex {
        uint32_t x;
        uint32_t y;

        bool operator==(const CellIndex &rhs) const noexcept {
            return x == rhs.x && y == rhs.y;
        }
    };

    /**
     * @enum Direction
     * @brief Defines the possible directions for movement or wall orientation in a grid system.
     *
     * The Direction enum is used to represent four primary cardinal directions:
     * - back: Represents movement or orientation towards the lower part of the grid (y - 1).
     * - left: Represents movement or orientation towards the left side of the grid (x - 1).
     * - front: Represents movement or orientation towards the upper part of the grid (y + 1).
     * - right: Represents movement or orientation towards the right side of the grid (x + 1).
     */
    enum Direction {
        Back = 0,
        Left = 1,
        Front = 2,
        Right = 3,
    };


    /**
     * @struct Cell
     * @brief Represents a cell in the maze grid with attributes for walls, adjacency,
     *        visit state, and utility functions for maze generation operations.
     *
     * The Cell struct encapsulates data and behaviors related to individual maze cells:
     * - Stores its unique index within the grid using CellIndex.
     * - Tracks whether the cell has been visited during maze generation.
     * - Encodes wall states using a bitmask, where each bit represents the presence of
     *   a wall in a specific direction (0 = down, 1 = left, 2 = up, 3 = right).
     * - Maintains a list of indices corresponding to adjacent cells in the grid.
     *
     * Additionally, the struct provides utility functions for working with walls,
     * identifying dead ends, and modifying the cell's wall configuration.
     */
    struct Cell {
        CellIndex cell_index;
        bool visited;
        uint8_t wall_bits;
        std::vector<CellIndex> adjacent_cells;

        [[nodiscard]] bool HasWall(const Direction dir) const noexcept {
            return (wall_bits & (1u << dir)) != 0;
        }

        [[nodiscard]] bool IsDeadEnd() const noexcept {
            int open_wall_count = 0;
            if (!HasWall(Left)) open_wall_count += 1;
            if (!HasWall(Right)) open_wall_count += 1;
            if (!HasWall(Front)) open_wall_count += 1;
            if (!HasWall(Back)) open_wall_count += 1;
            return open_wall_count == 1;
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
