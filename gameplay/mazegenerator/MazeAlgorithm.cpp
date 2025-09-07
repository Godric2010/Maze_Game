#include "MazeAlgorithm.hpp"

#include <ranges>

namespace Gameplay::Mazegenerator {
    MazeAlgorithm::MazeAlgorithm(const uint32_t width, const uint32_t height, const int seed) {
        m_grid_width = width;
        m_grid_height = height;
        m_seed = seed;
        m_rng = std::mt19937(seed);
        m_start_cell = {};
        m_exit_cell = {};
        m_cells = std::unordered_map<CellIndex, Cell>();
    }

    MazeAlgorithm::~MazeAlgorithm() = default;

    Maze MazeAlgorithm::GenerateMaze() {
        DefineCells();
        CarveMaze();
        BraidMaze();
        DefineCells();

        auto cells  = std::vector<Cell>();
        for (auto &cell: m_cells | std::views::values) {
            cells.push_back(cell);
        }

        return Maze(m_grid_width, m_grid_height, m_seed, cells, m_start_cell, m_exit_cell);
    }

    void MazeAlgorithm::DefineCells() {
        for (uint32_t x = 0; x < m_grid_width; ++x) {
            for (uint32_t y = 0; y < m_grid_height; ++y) {
                const auto cell_index = CellIndex(x, y);

                // collect all neighboring cells, sorted clockwise, beginning with the 'down' side
                auto neighbors = std::vector<CellIndex>();
                if (y != 0) {
                    neighbors.push_back(CellIndex(x, y - 1));
                }
                if (x != 0) {
                    neighbors.push_back(CellIndex(x - 1, y));
                }
                if (y != m_grid_height - 1) {
                    neighbors.push_back(CellIndex(x, y + 1));
                }
                if (x != m_grid_width - 1) {
                    neighbors.push_back(CellIndex(x + 1, y));
                }

                const auto cell = Cell{
                    .cell_index = cell_index,
                    .visited = false,
                    .wall_bits = (1u << 4) - 1,
                    .adjacent_cells = neighbors,
                };

                m_cells[cell_index] = cell;
            }
        }

        const uint32_t start_x = std::uniform_int_distribution<uint32_t>(0, m_grid_width - 1)(m_rng);
        m_start_cell = CellIndex(start_x, 0);
    }

    void MazeAlgorithm::CarveMaze() {
    }

    void MazeAlgorithm::BraidMaze() {
    }

    void MazeAlgorithm::DefinePoIs() {
    }
} // namespace
