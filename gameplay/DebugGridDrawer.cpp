#include "DebugGridDrawer.hpp"

#include <iostream>
#include <ostream>

namespace Gameplay {
    DebugGridDrawer::DebugGridDrawer() = default;

    DebugGridDrawer::~DebugGridDrawer() = default;

    void DebugGridDrawer::DrawGrid(Mazegenerator::Maze &maze) {
        m_start_idx = maze.entrance_cell;
        m_end_idx = maze.exit_cell;
        m_key_idx = maze.key_cell;

        std::string output;
        for (uint32_t y = maze.height; y > 0; --y) {
            for (uint32_t x = 0; x < maze.width; x++) {
                output += DrawCellTop(GetCell(maze.cells, x, y - 1));
            }
            output += "\n";
            for (uint32_t x = 0; x < maze.width; x++) {
                output += DrawCellMid(GetCell(maze.cells, x, y - 1));
            }
            output += "\n";
            for (uint32_t x = 0; x < maze.width; x++) {
                output += DrawCellBottom(GetCell(maze.cells, x, y - 1));
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

        std::string mid = " . ";
        if (c.visited) {
            mid = "   ";
        }
        if (c.cell_index == m_key_idx) {
            mid = " K ";
        } else if (c.cell_index == m_start_idx) {
            mid = " S ";
        } else if (c.cell_index == m_end_idx) {
            mid = " E ";
        }
        const std::string right = c.wall_bits & (1u << 3) ? "|" : " ";

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
