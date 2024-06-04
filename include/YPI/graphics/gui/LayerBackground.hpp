/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** LayerBackground
*/

#ifndef YPI_LAYERBACKGROUND_HPP
#define YPI_LAYERBACKGROUND_HPP

#include "../window/Window.hpp"
#include <vector>

namespace ypi::gui {

    class LayerBackground {
        public:
            LayerBackground();
            ~LayerBackground();

            void addLayer(const Vector2f& size, const Vector2f& position, sf::Texture& texture, const Vector2f& speed);
            void removeLayer(unsigned int index);
            void update(float dt);
            void render(RenderTarget& target);

        private:
            struct Layer {
                sf::RectangleShape shape;
                Vector2f speed;
                Vector2f lastPosition;
            };

            std::vector<Layer> m_layers;
    };
}

#endif //YPI_LAYERBACKGROUND_HPP