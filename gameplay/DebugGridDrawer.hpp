#pragma once
#include "mazegenerator/MazeAlgorithm.hpp"

namespace Gameplay {
    class DebugGridDrawer {
    public:
        DebugGridDrawer();

        ~DebugGridDrawer();

        void DrawGrid(Mazegenerator::Maze& maze);

    private:
        std::string DrawCellTop(const Mazegenerator::Cell& c);
        std::string DrawCellBottom(const Mazegenerator::Cell& c);
        std::string DrawCellMid(const Mazegenerator::Cell& c);

        Mazegenerator::Cell& GetCell(std::vector<Mazegenerator::Cell>& cells, uint32_t x, uint32_t y);

        Mazegenerator::CellIndex m_start_idx;
        Mazegenerator::CellIndex m_end_idx;
        Mazegenerator::CellIndex m_key_idx;
    };
} // namespace