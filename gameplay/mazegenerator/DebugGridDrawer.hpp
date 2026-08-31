#pragma once
#include "MazeAlgorithm.hpp"

namespace gameplay {
    class DebugGridDrawer {
    public:
        DebugGridDrawer();

        ~DebugGridDrawer();

        void DrawGrid(maze_generator::Maze& maze);

    private:
        std::string DrawCellTop(const maze_generator::Cell& c);
        std::string DrawCellBottom(const maze_generator::Cell& c);
        std::string DrawCellMid(const maze_generator::Cell& c);

        maze_generator::Cell& GetCell(std::vector<maze_generator::Cell>& cells, uint32_t x, uint32_t y);

        maze_generator::CellIndex m_start_idx{};
        maze_generator::CellIndex m_end_idx{};
        maze_generator::CellIndex m_key_idx{};
    };
} // namespace