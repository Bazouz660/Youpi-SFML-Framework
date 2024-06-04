#pragma once

#include "../components/components.hpp"

#include "YPI/ecs.hpp"
#include "YPI/data.hpp"
#include "YPI/math.hpp"
#include "YPI/graphics.hpp"

namespace sys
{

    class RenderGrid
    {
        public:
            RenderGrid(unsigned int width, unsigned int height, float cellSize)
            {
                m_vertexBuffer.setPrimitiveType(sf::Lines);
                m_vertexBuffer.create(width * height * 2);

                for (unsigned int i(0); i < width; ++i) {
                    for (unsigned int j(0); j < height; ++j) {
                        m_vertices.push_back(sf::Vertex());
                        m_vertices.push_back(sf::Vertex());
                        m_vertices.push_back(sf::Vertex());
                        m_vertices.push_back(sf::Vertex());

                        m_vertices.back().position = {i * cellSize, j * cellSize};
                        m_vertices[m_vertices.size() - 2].position = {(i + 1) * cellSize, j * cellSize};
                        m_vertices[m_vertices.size() - 3].position = {(i + 1) * cellSize, j * cellSize};
                        m_vertices[m_vertices.size() - 4].position = {(i + 1) * cellSize, (j + 1) * cellSize};

                        m_vertices.back().color = sf::Color::White;
                        m_vertices[m_vertices.size() - 2].color = sf::Color::White;
                        m_vertices[m_vertices.size() - 3].color = sf::Color::White;
                        m_vertices[m_vertices.size() - 4].color = sf::Color::White;

                        m_vertices.back().color.a = 50;
                        m_vertices[m_vertices.size() - 2].color.a = 50;
                        m_vertices[m_vertices.size() - 3].color.a = 50;
                        m_vertices[m_vertices.size() - 4].color.a = 50;
                    }
                }

                m_vertexBuffer.update(m_vertices.data());
            }

            void render(float offsetX, float offsetY, ypi::RenderWindow &window)
            {
                sf::RenderStates states;
                states.transform.translate(offsetX, offsetY);
                window.draw(m_vertexBuffer, states);
            }

        private:
            sf::VertexBuffer m_vertexBuffer;
            std::vector<sf::Vertex> m_vertices;
    };

}
