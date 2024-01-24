#pragma once

#include "ypi/src/core/vector2/Vector2.hpp"
#include "ypi/src/core/rect/Rect.hpp"

#include <cassert>
#include <algorithm>
#include <array>
#include <memory>
#include <type_traits>
#include <vector>

namespace exng
{

    template<typename T, typename GetBox, typename Equal = std::equal_to<T>, typename Float = float>
    class Grid {
    public:
        Grid(int width, int height, Float cellSize, GetBox getBox, Equal equal = Equal())
            : m_width(width), m_height(height), m_cellSize(cellSize), m_getBox(getBox), m_equal(equal) {
            m_cells.resize(m_width * m_height);
        }

        void insert(const T& item) {
            auto box = m_getBox(item);
            auto indices = getCellIndices(box);
            for (auto index : indices) {
                m_cells[index].push_back(item);
            }
        }

        void remove(const T& item) {
            auto box = m_getBox(item);
            auto indices = getCellIndices(box);
            for (auto index : indices) {
                auto& cell = m_cells[index];
                cell.erase(std::remove_if(cell.begin(), cell.end(), [&](const T& other) { return m_equal(item, other); }), cell.end());
            }
        }

        std::vector<T> query(const exng::Rect<Float>& queryBox) const {
            auto indices = getCellIndices(queryBox);
            std::vector<T> result;
            for (auto index : indices) {
                const auto& cell = m_cells[index];
                for (const auto& item : cell) {
                    auto itemBox = m_getBox(item);
                    if (queryBox.intersects(itemBox)) {
                        result.push_back(item);
                    }
                }
            }
            return result;
        }

        void clear() {
            for (auto& cell : m_cells) {
                cell.clear();
            }
        }

    private:
        std::vector<int> getCellIndices(const exng::Rect<Float>& box) const {
            int x1 = std::max(0, static_cast<int>(box.left / m_cellSize));
            int y1 = std::max(0, static_cast<int>(box.top / m_cellSize));
            int x2 = std::min(m_width - 1, static_cast<int>((box.left + box.width) / m_cellSize));
            int y2 = std::min(m_height - 1, static_cast<int>((box.top + box.height) / m_cellSize));
            std::vector<int> indices;
            for (int y = y1; y <= y2; ++y) {
                for (int x = x1; x <= x2; ++x) {
                    indices.push_back(y * m_width + x);
                }
            }
            return indices;
        }

        int m_width;
        int m_height;
        Float m_cellSize;
        GetBox m_getBox;
        Equal m_equal;
        std::vector<std::vector<T>> m_cells;
    };

} // namespace exng