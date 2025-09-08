#include "MazeAlgorithm.hpp"

#include <ranges>
#include <stack>

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
        DefinePoIs();

        auto cells = std::vector<Cell>();
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
        std::stack<CellIndex> m_path;

        CellIndex current_cell_idx = m_start_cell;
        m_path.push(current_cell_idx);

        while (!m_path.empty()) {
            Cell &current_cell = m_cells[current_cell_idx];
            current_cell.visited = true;
            auto next_idx = SelectNextCell(current_cell);
            if (!next_idx.has_value()) {
                if (auto fallback_idx = WalkBack(m_path); fallback_idx.has_value()) {
                    current_cell_idx = fallback_idx.value();
                    continue;
                }
                return;
            }
            const int dx = static_cast<int>(next_idx.value().x - current_cell_idx.x);
            const int dy = static_cast<int>(next_idx.value().y - current_cell_idx.y);

            Cell &next_cell = m_cells[next_idx.value()];
            OpenCellBorder(current_cell, dx, dy);
            OpenCellBorder(next_cell, dx * -1, dy * -1);

            current_cell_idx = next_idx.value();
            m_path.push(current_cell_idx);
        }
    }

    void MazeAlgorithm::BraidMaze() {
    }

    void MazeAlgorithm::DefinePoIs() {
    }

    std::optional<CellIndex> MazeAlgorithm::SelectNextCell(const Cell &current_cell) {
        std::vector<CellIndex> valid_neighbors;
        for (auto neighbor: current_cell.adjacent_cells) {
            if (!m_cells[neighbor].visited) {
                valid_neighbors.push_back(neighbor);
            }
        }

        if (valid_neighbors.empty()) {
            return std::nullopt;
        }

        const auto neighbor_idx = std::uniform_int_distribution<uint32_t>(0, valid_neighbors.size() - 1)(m_rng);
        const CellIndex next_cell_idx = valid_neighbors[neighbor_idx];
        return std::make_optional(next_cell_idx);
    }

    std::optional<CellIndex> MazeAlgorithm::WalkBack(std::stack<CellIndex> &stack) {
        while (!stack.empty()) {
            auto &top_cell_idx = stack.top();
            Cell &top_cell = m_cells[top_cell_idx];
            if (auto next_idx = SelectNextCell(top_cell); next_idx.has_value()) {
                return std::make_optional(top_cell_idx);
            }
            stack.pop();
        }
        return std::nullopt;
    }


    void MazeAlgorithm::OpenCellBorder(Cell &cell, const int dx, const int dy) {
        if (dx == 0 && dy == -1) {
            cell.wall_bits &= ~(1u << down);
            return;
        }
        if (dx == 0 && dy == 1) {
            cell.wall_bits &= ~(1u << up);
            return;
        }
        if (dx == -1 && dy == 0) {
            cell.wall_bits &= ~(1u << left);
            return;
        }
        if (dx == 1 && dy == 0) {
            cell.wall_bits &= ~(1u << right);
            return;
        }

        throw std::runtime_error("Delta was not correct!");
    }
} // namespace
