#pragma once

#include "../src/components/components.hpp"
#include "YPI/ecs.hpp"
#include "YPI/math.hpp"

using GridVal = std::pair<entt::entity, comp::Verlet*>;
using GetBoxFunc = std::function<ypi::Rect<float>(const GridVal&)>;

struct Equal {
    bool operator()(const std::pair<entt::entity, comp::Verlet*>& a, const std::pair<entt::entity, comp::Verlet*>& b) const {
        return a.first == b.first;
    }
};

struct PairHash {
    std::size_t operator()(const std::pair<entt::entity, comp::Verlet*>& pair) const {
        return std::hash<entt::entity>()(pair.first);
    }
};