/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** LayerBackground
*/

#include "LayerBackground.hpp"

namespace ypi::gui {

    LayerBackground::LayerBackground()
    {
    }

    LayerBackground::~LayerBackground()
    {
    }

    void LayerBackground::addLayer(const Vector2f& size, const Vector2f& position, sf::Texture& texture, const Vector2f& speed)
    {
        Layer layer;
        layer.shape.setSize(size);
        layer.shape.setPosition(position);
        layer.lastPosition = layer.shape.getPosition();
        layer.speed = speed;
        texture.setRepeated(true);
        layer.shape.setTexture(&texture);
        m_layers.push_back(layer);
    }

    void LayerBackground::removeLayer(unsigned int index)
    {
        m_layers.erase(m_layers.begin() + index);
    }

    void LayerBackground::update(float dt)
    {
        for (auto &layer : m_layers) {
            // compute new position by computing the average of the last position and the new position
            Vector2f position = {
                layer.shape.getTextureRect().left + layer.speed.x * 10 * dt,
                layer.shape.getTextureRect().top + layer.speed.y * 10 * dt,
            };

            Vector2f newPosition = {
                (layer.lastPosition.x + position.x) / 2,
                (layer.lastPosition.y + position.y) / 2
            };

            layer.shape.setTextureRect(sf::IntRect(
                newPosition.x,
                newPosition.y,
                layer.shape.getTextureRect().width,
                layer.shape.getTextureRect().height
            ));
            layer.lastPosition = newPosition;
        }
    }

    void LayerBackground::render(RenderTarget& target)
    {
        for (auto &layer : m_layers) {
            target.draw(layer.shape);
        }
    }
}