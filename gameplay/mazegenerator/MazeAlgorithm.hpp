#pragma once
#include <optional>
#include <random>
#include <stack>
#include <unordered_map>
#include <vector>

#include "Cell.hpp"

namespace Gameplay::Mazegenerator {
    /**
     * The maze struct contains all information needed to build a maze from.
     */
    struct Maze {
        uint32_t width;
        uint32_t height;
        int seed;
        std::vector<Cell> cells;
        CellIndex entrance_cell;
        CellIndex exit_cell;
        CellIndex key_cell;
    };

    /**
     * The MazeAlgorithm class provides the functionality to generate and configure a maze.
     * It includes methods to define cells, carve paths, braid dead ends, and specify points
     * of interest (PoIs) like the exit and key cells. It relies heavily on the usage of the dfs algorithm
     * to carve and follow paths, to calculate distances between various points.
     */
    class MazeAlgorithm {
    public:
        /**
         * Create the maze algorithm class with all necessary data to generate a maze.
         * @param width The width of the maze (number of cells on the x-axis)
         * @param height The height of the maze (number of cells on the y-axis)
         * @param seed The seed the rng will use to generate the maze.
         */
        MazeAlgorithm(uint32_t width, uint32_t height, int seed);

        ~MazeAlgorithm();

        /**
         * Create a mathematical representation of the maze.
         * @return The maze data that can be used to create a maze in 3D space.
         */
        Maze GenerateMaze();

    private:
        void DefineCells();

        void CarveMaze();

        void BraidMaze();

        void DefinePoIs(const std::vector<Cell> &cells);

        std::optional<CellIndex> SelectNextCell(const Cell& current_cell);

        std::optional<CellIndex> WalkBack(std::stack<CellIndex>& stack);

        static void OpenCellBorder(Cell& cell, int dx, int dy);

        CellIndex DefineExitCell();

        CellIndex DefineKeyCell(const std::vector<Cell> &cells);

        std::vector<int> CalculatePathLengths(CellIndex start);

        static uint32_t CellIndexAs1D(CellIndex idx, uint32_t grid_width);

        uint32_t m_grid_width;
        uint32_t m_grid_height;
        std::mt19937 m_rng;
        int m_seed;
        std::unordered_map<CellIndex, Cell> m_cells;
        std::vector<CellIndex> m_dead_ends;
        CellIndex m_start_cell{};
        CellIndex m_key_cell{};
        CellIndex m_exit_cell{};
    };
}

