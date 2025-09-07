#include "DebugGridDrawer.hpp"

#include <iostream>
#include <ostream>

namespace Gameplay {
    DebugGridDrawer::DebugGridDrawer() = default;

    DebugGridDrawer::~DebugGridDrawer() = default;

    void DebugGridDrawer::DrawGrid(Mazegenerator::Maze &maze) {
        std::string output;
        for (uint32_t y = 0; y < maze.height; y++) {
            for (uint32_t x = 0; x < maze.width; x++) {
                output += DrawCellTop(GetCell(maze.cells, x, y));
            }
            output += "\n";
            for (uint32_t x = 0; x < maze.width; x++) {
                output += DrawCellMid(GetCell(maze.cells, x, y));
            }
            output += "\n";
            for (uint32_t x = 0; x < maze.width; x++) {
                output += DrawCellBottom(GetCell(maze.cells, x, y));
            }
            output += "\n";
        }
        std::cout << output << std::endl;
    }

    std::string DebugGridDrawer::DrawCellTop(const Mazegenerator::Cell &c) {
        return std::string("+") + (c.wall_bits & (1u << 2) ? "---" : "   ") + "+";
    }

    std::string DebugGridDrawer::DrawCellBottom(const Mazegenerator::Cell &c) {
        return std::string("+") + (c.wall_bits & (1u << 0) ? "---" : "   ") + "+";
    }

    std::string DebugGridDrawer::DrawCellMid(const Mazegenerator::Cell &c) {
        std::string left = c.wall_bits & (1u << 1) ? "|" : " ";
        std::string mid = c.visited ? "   " : " . ";
        std::string right = c.wall_bits & (1u << 3) ? "|" : " ";

        return left + mid + right;
    }

    Mazegenerator::Cell &DebugGridDrawer::GetCell(std::vector<Mazegenerator::Cell> &cells, const uint32_t x,
                                                  const uint32_t y) {
        for (uint32_t i = 0; i < cells.size(); i++) {
            if (cells[i].cell_index.x == x && cells[i].cell_index.y == y) {
                return cells[i];
            }
        }
        throw std::invalid_argument("Cell does not exist");
    }
} // namespace
