#include "MazeAlgorithm.hpp"

#include <queue>
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
        m_key_cell = {};
        m_cells = std::unordered_map<CellIndex, Cell>();
    }

    MazeAlgorithm::~MazeAlgorithm() = default;

    Maze MazeAlgorithm::GenerateMaze() {
        DefineCells();
        CarveMaze();
        BraidMaze();

        auto cells = std::vector<Cell>();
        for (auto &cell: m_cells | std::views::values) {
            cells.push_back(cell);
        }
        DefinePoIs(cells);
        return Maze(m_grid_width, m_grid_height, m_seed, cells, m_start_cell, m_exit_cell, m_key_cell);
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

    // Carve maze section
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

    // Braid maze section
    void MazeAlgorithm::BraidMaze() {
    }


    // Define POIs section
    void MazeAlgorithm::DefinePoIs(const std::vector<Cell> &cells) {
        m_exit_cell = DefineExitCell();
        m_key_cell = DefineKeyCell(cells);
    }

    CellIndex MazeAlgorithm::DefineExitCell() {
        int best_x = 0;
        int best_dist = -1;

        const auto distances = CalculatePathLengths(m_start_cell);
        for (int x = 0; x < m_grid_width; ++x) {
            const int i = static_cast<int>(CellIndexAs1D(CellIndex(x, m_grid_height - 1), m_grid_width));
            if (distances[i] < best_dist) {
                continue;
            }
            best_dist = distances[i];
            best_x = x;
        }
        return CellIndex(best_x, m_grid_height - 1);
    }

    CellIndex MazeAlgorithm::DefineKeyCell(const std::vector<Cell> &cells) {
        std::vector<CellIndex> dead_ends;
        for (auto &cell: cells) {
            if (cell.IsDeadEnd()) {
                dead_ends.push_back(cell.cell_index);
            }
        }

        const auto distances_to_start = CalculatePathLengths(m_start_cell);
        const auto distances_to_end = CalculatePathLengths(m_exit_cell);

        CellIndex best_cell_idx{};
        double best_score = -1;

        for (const auto &cell: dead_ends) {
            const int i = static_cast<int>(CellIndexAs1D(cell, m_grid_width));
            int dist_to_start = distances_to_start[i];
            int dist_to_end = distances_to_end[i];

            const auto primary = std::min(dist_to_start, dist_to_end);
            const auto secondary = std::sqrt(dist_to_start * dist_to_end);
            if (const auto score = primary + secondary; score > best_score) {
                best_score = score;
                best_cell_idx = cell;
                continue;
            }
        }

        return best_cell_idx;
    }

    // Helper functions


    void MazeAlgorithm::OpenCellBorder(Cell &cell, const int dx, const int dy) {
        if (dx == 0 && dy == -1) {
            cell.RemoveWall(back);
            return;
        }
        if (dx == 0 && dy == 1) {
            cell.RemoveWall(front);
            return;
        }
        if (dx == -1 && dy == 0) {
            cell.RemoveWall(left);
            return;
        }
        if (dx == 1 && dy == 0) {
            cell.RemoveWall(right);
            return;
        }

        throw std::runtime_error("Delta was not correct!");
    }

    std::vector<int> MazeAlgorithm::CalculatePathLengths(const CellIndex start) {
        std::vector<int> dist(m_grid_width * m_grid_height, -1);
        std::queue<CellIndex> path;
        dist[CellIndexAs1D(start, m_grid_width)] = 0;
        path.push(start);

        while (!path.empty()) {
            CellIndex idx = path.front();
            path.pop();
            const Cell &current_cell = m_cells[idx];
            const int cell_dist = dist[CellIndexAs1D(idx, m_grid_width)];

            auto try_push = [&](const CellIndex next) {
                if (next.x < 0 || next.x >= m_grid_width || next.y < 0 || next.y >= m_grid_height) {
                    return;
                }
                const uint32_t next_idx = CellIndexAs1D(next, m_grid_width);
                if (dist[next_idx] != -1) return;
                dist[next_idx] = cell_dist + 1;
                path.push(next);
            };

            if (!current_cell.HasWall(back)) {
                try_push(CellIndex{idx.x, idx.y - 1});
            }
            if (!current_cell.HasWall(front)) {
                try_push(CellIndex{idx.x, idx.y + 1});
            }
            if (!current_cell.HasWall(right)) {
                try_push(CellIndex{idx.x + 1, idx.y});
            }
            if (!current_cell.HasWall(left)) {
                try_push(CellIndex{idx.x - 1, idx.y});
            }
        }
        return dist;
    }

    uint32_t MazeAlgorithm::CellIndexAs1D(const CellIndex idx, const uint32_t grid_width) {
        return idx.y * grid_width + idx.x;
    }
} // namespace
