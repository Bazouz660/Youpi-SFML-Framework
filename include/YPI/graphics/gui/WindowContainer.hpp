#ifndef YPI_GUI_WINDOW_CONTAINER_HPP
#define YPI_GUI_WINDOW_CONTAINER_HPP

#include "Container.hpp"
#include "ScrollBar.hpp"

namespace ypi::gui
{

    class WindowContainer : public Container {
    protected:
        sf::RenderTexture m_renderTexture;
        sf::RectangleShape m_sprite;
        Vector2f m_size;
        Vector2f m_fixedViewCenter;
        Vector2f m_position;
        sf::View m_view;
        Container m_scrollContainer;
        std::shared_ptr<LambdaObserver<float>> m_scrollHObserver;
        std::shared_ptr<LambdaObserver<float>> m_scrollVObserver;
        bool m_fixedView = false;

        FloatRect getContentBounds() const;
        void updateView(float progX, float progY);
        void updateViewX(float progX);
        void updateViewY(float progY);

        virtual void imp_setPosition() override;

    public:

        enum class ScrollBarSideH {
            TOP,
            BOTTOM
        };

        enum class ScrollBarSideV {
            LEFT,
            RIGHT
        };

        WindowContainer();
        virtual ~WindowContainer() = default;

        void setSize(const Vector2f& size);
        Vector2f getSize() const;
        void setScrollBarSideH(ScrollBarSideH side);
        void setScrollBarSideV(ScrollBarSideV side);
        void setBackgroundColor(const sf::Color& color);
        void setBackgroundOutlineColor(const sf::Color& color);
        void setBackgroundOutlineThickness(float thickness);

        void enableFixedView(const Vector2f& center);
        void disableFixedView();

        virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
        virtual void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;
        virtual void render(RenderTarget& target) override;

        virtual void addComponent(const std::string& key, const_sptr_ref<AComponent> component, int zIndex = 0);
        virtual void removeComponent(const std::string& key);

        virtual FloatRect getGlobalBounds() const override;

        protected:
            ScrollBarSideH m_scrollBarSideH = ScrollBarSideH::BOTTOM;
            ScrollBarSideV m_scrollBarSideV = ScrollBarSideV::RIGHT;

            std::shared_ptr<ScrollBar> m_scrollBarH = nullptr;
            std::shared_ptr<ScrollBar> m_scrollBarV = nullptr;

            void setScrollBarHPosition();
            void setScrollBarVPosition();
    };
}

#endif //YPI_GUI_WINDOW_CONTAINER_HPP