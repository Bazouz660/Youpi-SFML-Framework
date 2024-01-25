#pragma once

#include "ypi/src/core/vector2/Vector2.hpp"
#include "ypi/src/core/rect/Rect.hpp"

#include <cassert>
#include <algorithm>
#include <array>
#include <memory>
#include <type_traits>
#include <vector>
#include <unordered_map>

namespace exng
{

template<typename ItemType, typename GetBox, typename Equal = std::equal_to<ItemType>, typename Hash = std::hash<ItemType>, typename Float = float>
class Grid {
public:
    Grid(int width, int height, Float cellSize, GetBox getBox, Equal equal = Equal())
        : m_width(width), m_height(height), m_cellSize(cellSize), m_getBox(getBox), m_equal(equal) {
            m_grid.resize(m_width * m_height);
    }

    void insert(const ItemType& item) {
        auto box = m_getBox(item);
        int xStart = std::max(0, static_cast<int>(box.left / m_cellSize));
        int yStart = std::max(0, static_cast<int>(box.top / m_cellSize));
        int xEnd = std::min(m_width - 1, static_cast<int>((box.left + box.width) / m_cellSize));
        int yEnd = std::min(m_height - 1, static_cast<int>((box.top + box.height) / m_cellSize));

        for (int x = xStart; x <= xEnd; ++x) {
            for (int y = yStart; y <= yEnd; ++y) {
                m_grid[y * m_width + x].push_back(item);
                m_itemPositions[item].push_back({x, y});
            }
        }
    }

    // updates the grid for the given item, if the items is not in the grid, it is inserted
    void update(const ItemType& item) {
        auto box = m_getBox(item);
        int xStart = std::max(0, static_cast<int>(box.left / m_cellSize));
        int yStart = std::max(0, static_cast<int>(box.top / m_cellSize));
        int xEnd = std::min(m_width - 1, static_cast<int>((box.left + box.width) / m_cellSize));
        int yEnd = std::min(m_height - 1, static_cast<int>((box.top + box.height) / m_cellSize));

        auto& positions = m_itemPositions[item];
        for (auto& position : positions) {
            auto& cell = m_grid[position.y * m_width + position.x];
            auto it = std::find(cell.begin(), cell.end(), item);
            if (it != cell.end()) {
                cell.erase(it);
            }
        }
        positions.clear();

        for (int x = xStart; x <= xEnd; ++x) {
            for (int y = yStart; y <= yEnd; ++y) {
                m_grid[y * m_width + x].push_back(item);
                positions.push_back({x, y});
            }
        }
    }

    void clear() {
        for (auto& cell : m_grid) {
            cell.clear();
        }
    }

    std::vector<ItemType> query(const Rect<Float>& box) const {
        int xStart = std::max(0, static_cast<int>(box.left / m_cellSize));
        int yStart = std::max(0, static_cast<int>(box.top / m_cellSize));
        int xEnd = std::min(m_width - 1, static_cast<int>((box.left + box.width) / m_cellSize));
        int yEnd = std::min(m_height - 1, static_cast<int>((box.top + box.height) / m_cellSize));

        std::vector<ItemType> result;
        for (int x = xStart; x <= xEnd; ++x) {
            for (int y = yStart; y <= yEnd; ++y) {
                const auto& cell = m_grid[y * m_width + x];
                result.insert(result.end(), cell.begin(), cell.end());
            }
        }

        return result;
    }

    std::vector<ItemType> query(const exng::Vector2f& position) const {
        int x = std::max(0, static_cast<int>(position.x / m_cellSize));
        int y = std::max(0, static_cast<int>(position.y / m_cellSize));

        const auto& cell = m_grid[y * m_width + x];
        return cell;
    }

private:
    int m_width;
    int m_height;
    Float m_cellSize;
    GetBox m_getBox;
    Equal m_equal;
    std::vector<std::vector<ItemType>> m_grid;

    struct Position {
        int x;
        int y;
    };

    std::unordered_map<ItemType, std::vector<Position>, Hash> m_itemPositions;
};

} // namespace exng