/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Slider
*/

#ifndef YPI_GUI_SLIDER_HPP
#define YPI_GUI_SLIDER_HPP

#include "AComponent.hpp"
#include "../../system/Observer.hpp"

namespace ypi::gui
{
    class Slider : virtual public AComponent, public Subject<float>
    {
    public:
        enum class Alignment {
            HORIZONTAL,
            VERTICAL
        };

        enum class State {
            Idle,
            Hovered,
            Grabbed
        };

    protected:
        float m_progress = 0; /**< La valeur de progression de la barre. */
        float m_delta = 0.05f; /**< La valeur de progression à chaque déplacement de la molette. */
        std::vector<float> m_snapPoints;
        Vector2f m_grabOffset = {0, 0}; /**< La position relative de la souris par rapport au centre du curseur. */
        sf::RectangleShape m_cart; /**< La forme rectangulaire de la barre. */
        Alignment m_alignment = Alignment::HORIZONTAL;
        State m_state = State::Idle;

        void updateCartPosition();

        void handleScrolling(const sf::Event& event, const Vector2f& mousePos);
        void handleGrabbing(const sf::Event& event, const Vector2f& mousePos);

        virtual void imp_setPosition() override;

    public:

        Slider();
        ~Slider() override = default;
        float getProgress() const;
        void setProgress(float progress);

        void setSize(const Vector2f& size);
        void setCartSize(const unsigned int& size);
        void setAlignment(Alignment alignment);
        sf::RectangleShape& getCart();
        void addSnapPoint(float snapPoint);
        void setSnapPoints(const std::vector<float>& snapPoints);
        void clearSnapPoints();

        void setCartColor(const sf::Color& color);
        void setCartOutlineColor(const sf::Color& color);
        void setCartOutlineThickness(float thickness);
        void setCartTexture(const sf::Texture& texture);

        void setBarColor(const sf::Color& color);
        void setBarOutlineColor(const sf::Color& color);
        void setBarOutlineThickness(float thickness);
        void setBarTexture(const sf::Texture& texture);

        virtual void render(RenderTarget& target) override;
        virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
        virtual FloatRect getGlobalBounds() const override;
    };
}

#endif //YPI_GUI_SLIDER_HPP
