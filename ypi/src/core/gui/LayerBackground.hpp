/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** LayerBackground
*/

#ifndef EXNG_LAYERBACKGROUND_HPP
#define EXNG_LAYERBACKGROUND_HPP

#include "core/window/Window.hpp"
#include <vector>

namespace exng::gui {

    class LayerBackground {
        public:
            LayerBackground();
            ~LayerBackground();

            void addLayer(const Vector2f& size, const Vector2f& position, sf::Texture& texture, const Vector2f& speed);
            void removeLayer(unsigned int index);
            void update(float dt);
            void render(exng::Window& window);

        private:
            struct Layer {
                sf::RectangleShape shape;
                Vector2f speed;
                Vector2f lastPosition;
            };

            std::vector<Layer> m_layers;
    };
}

#endif //EXNG_LAYERBACKGROUND_HPP