#include "WindowContainer.hpp"
#include "logger.hpp"

namespace ypi::gui
{
    WindowContainer::WindowContainer()
    {
        m_size = {300, 300};
        m_position = {0, 0};
        m_renderTexture.create(m_size.x, m_size.y);
        m_view.setSize(m_size);
        m_view.setViewport({0, 0, 1, 1});
        m_renderTexture.setView(m_view);

        m_shape.setSize(m_size);
        m_shape.setFillColor(sf::Color(30, 30, 30, 100));

        auto sliderH = std::make_shared<ScrollBar>();
        m_scrollBarH = sliderH;
        sliderH->setAlignment(ScrollBar::Alignment::HORIZONTAL);
        sliderH->setCartSize(8);
        sliderH->setSize({m_size.x - 40, 10});
        sliderH->setBarColor(sf::Color(75, 75, 75, 200));
        sliderH->setCartColor(sf::Color(200, 200, 200, 255));

        auto sliderV = std::make_shared<ScrollBar>();
        m_scrollBarV = sliderV;
        sliderV->setAlignment(ScrollBar::Alignment::VERTICAL);
        sliderV->setCartSize(8);
        sliderV->setSize({10, m_size.y - 40});
        sliderV->setBarColor(sf::Color(75, 75, 75, 200));
        sliderV->setCartColor(sf::Color(200, 200, 200, 255));

        m_scrollContainer.addComponent("sliderH", sliderH);
        m_scrollContainer.addComponent("sliderV", sliderV);

        m_scrollHObserver = std::make_shared<LambdaObserver<float>>([this](float progress){
            updateViewX(progress);
        });

        m_scrollVObserver = std::make_shared<LambdaObserver<float>>([this](float progress){
            updateViewY(progress);
        });


        sliderH->addObserver(m_scrollHObserver);
        setScrollBarHPosition();

        sliderV->addObserver(m_scrollVObserver);
        setScrollBarVPosition();

        updateView(sliderH->getProgress(), sliderV->getProgress());
    }

    void WindowContainer::updateView(float progX, float progY)
    {
        FloatRect contentBounds = getContentBounds();
        float centerX;
        float centerY;

        if (m_fixedView) {
            centerX = m_fixedViewCenter.x + (progX * (contentBounds.width - m_size.x * 0.9f));
            centerX += m_absolutePosition.x;
            centerY = m_fixedViewCenter.y + (progY * (contentBounds.height - m_size.y * 0.9f));
            centerY += m_absolutePosition.y;
        } else {
            centerX = (contentBounds.left) + (progX * (contentBounds.width - m_size.x * 0.9f));
            centerX += m_size.x * 0.5f;
            centerY = (contentBounds.top) + (progY * (contentBounds.height - m_size.y * 0.9f));
            centerY += m_size.y * 0.5f;
        }

        m_view.setCenter(centerX , centerY);
        m_renderTexture.setView(m_view);
    }

    void WindowContainer::updateViewX(float progX)
    {
        updateView(progX, m_scrollBarV->getProgress());
    }

    void WindowContainer::updateViewY(float progY)
    {
        updateView(m_scrollBarH->getProgress(), progY);
    }

    void WindowContainer::setSize(const Vector2f& size)
    {
        m_size = size;
        m_renderTexture.create(m_size.x, m_size.y);
        m_view.setSize(m_size);
        m_view.setViewport({0, 0, 1, 1});
        m_renderTexture.setView(m_view);

        m_shape.setSize(m_size);

        m_scrollBarH->setSize({m_size.x - 40, 10});
        m_scrollBarV->setSize({10, m_size.y - 40});
        setScrollBarHPosition();
        setScrollBarVPosition();

        updateView(m_scrollBarH->getProgress(), m_scrollBarV->getProgress());
    }

    void WindowContainer::render(RenderTarget& target)
    {
        target.draw(m_shape);
        m_renderTexture.clear(sf::Color::Transparent);
        Container::render(m_renderTexture);
        m_renderTexture.display();
        //m_sprite.setOutlineColor(sf::Color::Blue);
        //m_sprite.setOutlineThickness(1);
        m_sprite.setSize(Vector2f(m_renderTexture.getSize().x, m_renderTexture.getSize().y));
        m_sprite.setTexture(&m_renderTexture.getTexture());
        m_sprite.setPosition(m_position);
        target.draw(m_sprite);

        m_scrollContainer.render(target);
    }

    void WindowContainer::imp_setPosition()
    {
        m_position = m_absolutePosition;
        m_shape.setPosition(m_absolutePosition);
        m_renderTexture.setView(m_view);

        m_scrollContainer.setPosition(m_absolutePosition);

        updateView(m_scrollBarH->getProgress(), m_scrollBarV->getProgress());
    }

    FloatRect WindowContainer::getGlobalBounds() const
    {
        FloatRect bounds;
        bounds.left = m_position.x;
        bounds.top = m_position.y;
        bounds.width = m_size.x;
        bounds.height = m_size.y;
        bounds = bounds.combine(m_scrollContainer.getGlobalBounds());
        return bounds;
    }

    void WindowContainer::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        Vector2f localMousePos = m_renderTexture.mapPixelToCoords(sf::Vector2i(mousePos.x, mousePos.y));
        localMousePos -= m_position;
        Container::handleAnyEvents(event, localMousePos);
        m_scrollContainer.handleAnyEvents(event, mousePos);

        FloatRect contentBounds = getContentBounds();
        m_scrollBarH->setScrollableRatio(m_size.x / contentBounds.width);
        m_scrollBarV->setScrollableRatio(m_size.y / contentBounds.height);
    }

    void WindowContainer::handleHoverEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        Vector2f localMousePos = m_renderTexture.mapPixelToCoords(sf::Vector2i(mousePos.x, mousePos.y));
        localMousePos -= m_position;
        Container::handleHoverEvents(event, localMousePos);
        m_scrollContainer.handleHoverEvents(event, mousePos);
    }

    FloatRect WindowContainer::getContentBounds() const
    {
        return Container::getGlobalBounds();
    }

    void WindowContainer::addComponent(const std::string& key, const_sptr_ref<AComponent> component, int zIndex)
    {
        Container::addComponent(key, component, zIndex);
        FloatRect contentBounds = getContentBounds();
        m_scrollBarH->setScrollableRatio(m_size.x / contentBounds.width);
        m_scrollBarV->setScrollableRatio(m_size.y / contentBounds.height);
        updateView(m_scrollBarH->getProgress(), m_scrollBarV->getProgress());
    }

    void WindowContainer::removeComponent(const std::string& key)
    {
        Container::removeComponent(key);
        FloatRect contentBounds = getContentBounds();
        m_scrollBarH->setScrollableRatio(m_size.x / contentBounds.width);
        m_scrollBarV->setScrollableRatio(m_size.y / contentBounds.height);
        updateView(m_scrollBarH->getProgress(), m_scrollBarV->getProgress());
    }

    void WindowContainer::setScrollBarSideH(ScrollBarSideH side)
    {
        m_scrollBarSideH = side;
        setScrollBarHPosition();
    }

    void WindowContainer::setScrollBarSideV(ScrollBarSideV side)
    {
        m_scrollBarSideV = side;
        setScrollBarVPosition();
    }

    void WindowContainer::setScrollBarHPosition()
    {
        if (m_scrollBarSideH == ScrollBarSideH::TOP)
            m_scrollBarH->setPosition({m_size.x * 0.5f, m_scrollBarH->getGlobalBounds().height * 0.5f});
        else
            m_scrollBarH->setPosition({m_size.x * 0.5f, m_size.y - m_scrollBarH->getGlobalBounds().height * 0.5f});
    }

    void WindowContainer::setScrollBarVPosition()
    {
        if (m_scrollBarSideV == ScrollBarSideV::LEFT)
            m_scrollBarV->setPosition({m_scrollBarV->getGlobalBounds().width * 0.5f, m_size.y * 0.5f});
        else
            m_scrollBarV->setPosition({m_size.x - m_scrollBarV->getGlobalBounds().width * 0.5f, m_size.y * 0.5f});
    }

    void WindowContainer::setBackgroundColor(const sf::Color& color)
    {
        m_shape.setFillColor(color);
    }

    void WindowContainer::setBackgroundOutlineColor(const sf::Color& color)
    {
        m_shape.setOutlineColor(color);
    }

    void WindowContainer::setBackgroundOutlineThickness(float thickness)
    {
        m_shape.setOutlineThickness(thickness);
    }

    Vector2f WindowContainer::getSize() const
    {
        return m_size;
    }

    void WindowContainer::enableFixedView(const Vector2f& center)
    {
        m_fixedView = true;
        m_fixedViewCenter = center;
    }

    void WindowContainer::disableFixedView()
    {
        m_fixedView = false;
    }

} // namespace ypi::gui