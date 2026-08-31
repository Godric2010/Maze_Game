#include "SystemMetaSorter.hpp"
#include <queue>

namespace yarep::ecs {
    std::vector<SystemMeta> SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(
            const std::vector<SystemMeta>& system_metas) {
        using PhaseType = std::underlying_type_t<Phase>;

        struct QueueEntry {
            std::size_t index;
        };

        auto cmp = [&system_metas](const QueueEntry& a, const QueueEntry& b) {
            const auto& lhs = system_metas[a.index];
            const auto& rhs = system_metas[b.index];

            const auto lhs_phase = static_cast<PhaseType>(lhs.phase);
            const auto rhs_phase = static_cast<PhaseType>(rhs.phase);

            if (lhs_phase != rhs_phase) {
                return lhs_phase > rhs_phase; // min-heap Verhalten über priority_queue
            }

            return lhs.name > rhs.name;
        };

        std::unordered_map<std::string, std::size_t> name_to_index;
        name_to_index.reserve(system_metas.size());

        for (std::size_t i = 0; i < system_metas.size(); ++i) {
            const auto inserted = name_to_index.emplace(system_metas[i].name, i);
            if (!inserted.second) {
                throw std::runtime_error("Duplicate system name detected: " + system_metas[i].name);
            }
        }

        std::vector<std::vector<std::size_t> > outgoing_edges(system_metas.size());
        std::vector<std::size_t> in_degree(system_metas.size(), 0);

        for (std::size_t i = 0; i < system_metas.size(); ++i) {
            for (const auto& dependency_name: system_metas[i].dependencies) {
                const auto it = name_to_index.find(dependency_name);
                if (it == name_to_index.end()) {
                    throw std::runtime_error(
                            "System '" + system_metas[i].name + "' depends on unknown system '" + dependency_name + "'"
                            );
                }

                const std::size_t dependency_index = it->second;

                if (dependency_index == i) {
                    throw std::runtime_error(
                            "System '" + system_metas[i].name + "' must not depend on itself"
                            );
                }

                // dependency -> current system
                outgoing_edges[dependency_index].push_back(i);
                ++in_degree[i];

                const auto dependency_phase = static_cast<PhaseType>(system_metas[dependency_index].phase);
                const auto current_phase = static_cast<PhaseType>(system_metas[i].phase);

                if (dependency_phase > current_phase) {
                    throw std::runtime_error(
                            "Phase violation: system '" + system_metas[i].name +
                            "' depends on '" + dependency_name +
                            "', but dependency phase is later than dependent phase"
                            );
                }
            }
        }

        std::priority_queue<QueueEntry, std::vector<QueueEntry>, decltype(cmp)> ready(cmp);

        for (std::size_t i = 0; i < system_metas.size(); ++i) {
            if (in_degree[i] == 0) {
                ready.push(QueueEntry{i});
            }
        }

        std::vector<SystemMeta> result;
        result.reserve(system_metas.size());

        while (!ready.empty()) {
            const auto current = ready.top();
            ready.pop();

            result.push_back(system_metas[current.index]);

            for (const auto next_index: outgoing_edges[current.index]) {
                --in_degree[next_index];
                if (in_degree[next_index] == 0) {
                    ready.push(QueueEntry{next_index});
                }
            }
        }

        if (result.size() != system_metas.size()) {
            throw std::runtime_error("Cycle detected in system dependencies");
        }

        return result;
    }
} // namespace
