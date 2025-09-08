#pragma once
#include <random>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <cstdint>
#include <stack>

#include "Cell.hpp"

namespace Gameplay::Mazegenerator {
    struct Maze {
        uint32_t width;
        uint32_t height;
        int seed;
        std::vector<Cell> cells;
        CellIndex entrance_cell;
        CellIndex exit_cell;
    };

    class MazeAlgorithm {
    public:
        MazeAlgorithm(uint32_t width, uint32_t height, int seed);

        ~MazeAlgorithm();

        Maze GenerateMaze();

    private:
        void DefineCells();

        void CarveMaze();

        void BraidMaze();

        void DefinePoIs();

        std::optional<CellIndex> SelectNextCell(const Cell& current_cell);

        std::optional<CellIndex> WalkBack(std::stack<CellIndex>& stack);

        static void OpenCellBorder(Cell& cell, int dx, int dy);

        uint32_t m_grid_width;
        uint32_t m_grid_height;
        std::mt19937 m_rng;
        int m_seed;
        std::unordered_map<CellIndex, Cell> m_cells;
        std::vector<CellIndex> m_dead_ends;
        CellIndex m_start_cell;
        CellIndex m_exit_cell;
    };
}

