#include "SystemMetaSorter.hpp"
#include <queue>

namespace Engine::Ecs {
    std::vector<SystemMeta> SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(
            const std::vector<SystemMeta>& system_metas) {
        using PhaseType = std::underlying_type_t<Phase>;

        struct QueueEntry {
            std::size_t index;
        };

        auto cmp = [&system_metas](const QueueEntry& a, const QueueEntry& b) {
            const auto& lhs = system_metas[a.index];
            const auto& rhs = system_metas[b.index];

            const auto lhsPhase = static_cast<PhaseType>(lhs.phase);
            const auto rhsPhase = static_cast<PhaseType>(rhs.phase);

            if (lhsPhase != rhsPhase) {
                return lhsPhase > rhsPhase; // min-heap Verhalten über priority_queue
            }

            return lhs.name > rhs.name;
        };

        std::unordered_map<std::string, std::size_t> nameToIndex;
        nameToIndex.reserve(system_metas.size());

        for (std::size_t i = 0; i < system_metas.size(); ++i) {
            const auto inserted = nameToIndex.emplace(system_metas[i].name, i);
            if (!inserted.second) {
                throw std::runtime_error("Duplicate system name detected: " + system_metas[i].name);
            }
        }

        std::vector<std::vector<std::size_t> > outgoingEdges(system_metas.size());
        std::vector<std::size_t> inDegree(system_metas.size(), 0);

        for (std::size_t i = 0; i < system_metas.size(); ++i) {
            for (const auto& dependencyName: system_metas[i].dependencies) {
                const auto it = nameToIndex.find(dependencyName);
                if (it == nameToIndex.end()) {
                    throw std::runtime_error(
                            "System '" + system_metas[i].name + "' depends on unknown system '" + dependencyName + "'"
                            );
                }

                const std::size_t dependencyIndex = it->second;

                if (dependencyIndex == i) {
                    throw std::runtime_error(
                            "System '" + system_metas[i].name + "' must not depend on itself"
                            );
                }

                // dependency -> current system
                outgoingEdges[dependencyIndex].push_back(i);
                ++inDegree[i];

                const auto dependencyPhase = static_cast<PhaseType>(system_metas[dependencyIndex].phase);
                const auto currentPhase = static_cast<PhaseType>(system_metas[i].phase);

                if (dependencyPhase > currentPhase) {
                    throw std::runtime_error(
                            "Phase violation: system '" + system_metas[i].name +
                            "' depends on '" + dependencyName +
                            "', but dependency phase is later than dependent phase"
                            );
                }
            }
        }

        std::priority_queue<QueueEntry, std::vector<QueueEntry>, decltype(cmp)> ready(cmp);

        for (std::size_t i = 0; i < system_metas.size(); ++i) {
            if (inDegree[i] == 0) {
                ready.push(QueueEntry{i});
            }
        }

        std::vector<SystemMeta> result;
        result.reserve(system_metas.size());

        while (!ready.empty()) {
            const auto current = ready.top();
            ready.pop();

            result.push_back(system_metas[current.index]);

            for (const auto nextIndex: outgoingEdges[current.index]) {
                --inDegree[nextIndex];
                if (inDegree[nextIndex] == 0) {
                    ready.push(QueueEntry{nextIndex});
                }
            }
        }

        if (result.size() != system_metas.size()) {
            throw std::runtime_error("Cycle detected in system dependencies");
        }

        return result;
    }
} // namespace
