#include "DropDownList.hpp"
#include "logger.hpp"
#include "context.hpp"
#include "Shaders.hpp"

namespace ypi::gui
{
    template <typename T>
    DropDownList<T>::DropDownList()
    {
        setCallback([this](){
            toggle();
        });

        setPlaceholder(m_placeholder);
        updateView();

        m_scrollBar = std::make_shared<ScrollBar>();
        m_scrollBar->setAlignment(ScrollBar::Alignment::VERTICAL);
        m_itemsContainer.addComponent("scrollBar", m_scrollBar, 1);

        m_scrollBarObserver = std::make_shared<LambdaObserver<float>>([this](float progress){
            m_itemsOffset.y = progress * (m_items.size() * m_shape.getGlobalBounds().height - m_itemsView.getSize().y);
            updateItemsPosition();
        });
        m_scrollBar->addObserver(m_scrollBarObserver);

        updateScrollbarSize();
        updateScrollbarPosition();
    }

    template <typename T>
    void DropDownList<T>::updateItemsSize()
    {
        for (auto& [key, item] : m_items) {
            item->setBackgroundSize(m_shape.getSize());
        }
    }

    template <typename T>
    void DropDownList<T>::setBackgroundSize(const Vector2f& size)
    {
        Button::setBackgroundSize(size);
        updateItemsSize();
        updateItemsPosition();
        updateScrollbarSize();
        updateScrollbarPosition();
        updateView();
    }

    template <typename T>
    void DropDownList<T>::render(RenderTarget& target)
    {
        Button::render(target);

        if (m_isOpen)
        {

            m_panel.setSize(Vector2f(m_itemsTexture.getSize().x, std::min(m_maxShownItems, (unsigned int)m_items.size()) * m_shape.getSize().y));
            m_panel.setFillColor(sf::Color(255, 255, 255, 255));

            sf::Texture panelTexture;
            panelTexture.create(m_panel.getSize().x, m_panel.getSize().y);
            sf::Image whiteImage;
            sf::Color color = m_panelColor;
            color.a = 240;
            whiteImage.create(m_panel.getSize().x, m_panel.getSize().y, color);
            panelTexture.update(whiteImage);
            panelTexture.setRepeated(true);

            m_panel.setTexture(&panelTexture);

            sf::RenderStates states = sf::RenderStates::Default;
            states.shader = effect::getFadeBorders(0.1f, 0.05f);
            states.blendMode = sf::BlendAlpha;

            m_itemsTexture.clear(sf::Color::Transparent);
                m_itemsContainer.render(m_itemsTexture);
            m_itemsTexture.display();

            m_itemsSprite.setSize(Vector2f(m_itemsTexture.getSize().x, m_itemsTexture.getSize().y));
            m_itemsSprite.setTexture(&m_itemsTexture.getTexture());
            m_itemsSprite.setFillColor(sf::Color::White);
            m_itemsSprite.setPosition({m_shape.getGlobalBounds().left, m_shape.getGlobalBounds().top + m_shape.getGlobalBounds().height});
            m_panel.setPosition(m_itemsSprite.getPosition());

            target.draw(m_panel, states);

            states.shader = effect::getFadeBorders(0.0f, 0.1f);
            target.draw(m_itemsSprite, states);
            m_scrollBar->render(target);
        }
    }

    template <typename T>
    void DropDownList<T>::updateItemsPosition()
    {
        int i = 0;
        for (auto& item : m_orderedItems) {
            item->setPosition({getPosition().x, (getPosition().y + m_shape.getGlobalBounds().height + m_shape.getGlobalBounds().height * i++) - m_itemsOffset.y});
        }
    }

    template <typename T>
    void DropDownList<T>::handleHoverEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        Button::handleHoverEvents(event, mousePos);
        if (m_isOpen)
        {
            FloatRect bounds = {m_itemsView.getCenter().x - m_itemsView.getSize().x / 2.f,
                m_itemsView.getCenter().y - m_itemsView.getSize().y / 2.f,
                m_itemsView.getSize().x, m_itemsView.getSize().y};

            if (bounds.contains(mousePos)) {
                m_itemsContainer.handleHoverEvents(event, mousePos);
            } else {
                m_scrollBar->handleHoverEvents(event, mousePos);
            }
        }
    }

    template <typename T>
    void DropDownList<T>::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        Button::handleAnyEvents(event, mousePos);
        if (m_isOpen)
        {
            if (event.type == sf::Event::MouseButtonPressed && !getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                close();
                return;
            }

            m_itemsContainer.handleAnyEvents(event, mousePos);
            m_scrollBar->handleAnyEvents(event, mousePos);
        }
    }

    template <typename T>
    void DropDownList<T>::updateScrollbarPosition()
    {
        m_scrollBar->setPosition({m_shape.getGlobalBounds().left + m_shape.getGlobalBounds().width - (m_scrollBar->getGlobalBounds().width),
            m_shape.getGlobalBounds().top + m_shape.getGlobalBounds().height + m_scrollBar->getGlobalBounds().height * 0.5f});
    }

    template <typename T>
    void DropDownList<T>::updateScrollbarSize()
    {
        m_scrollBar->setScrollableRatio((float)m_maxShownItems / m_items.size());
        m_scrollBar->setSize({(float)m_scrollBarWidth, m_shape.getGlobalBounds().height * m_maxShownItems});
    }

    template <typename T>
    void DropDownList<T>::imp_setPosition()
    {
        Button::imp_setPosition();

        updateItemsPosition();
        updateScrollbarPosition();
        updateView();
    }

    template <typename T>
    FloatRect DropDownList<T>::getGlobalBounds() const
    {
        FloatRect bounds = Button::getGlobalBounds();
        if (m_isOpen) {
            bounds.height += std::min(m_maxShownItems, (unsigned int)m_items.size()) * m_shape.getSize().y;
        }
        return bounds;
    }

    template <typename T>
    void DropDownList<T>::addItem(const sf::String& name, T value)
    {
        if (name == "scrollBar")
        {
            logger::error() << "Cannot add item with name 'scrollBar', it's a reserved name.";
            return;
        }

        m_items[name] = std::make_shared<DropDownItem<T>>();
        auto& item = m_items[name];
        m_orderedItems.push_back(item);

        item->getShape() = m_shape;

        item->getLabel().setFont(getLabel().getFont());
        item->getLabel().setCharacterSize(getLabel().getCharacterSize());
        item->getLabel().setFillColor(getLabel().getFillColor());
        item->getLabel().setOutlineColor(getLabel().getOutlineColor());
        item->getLabel().setOutlineThickness(getLabel().getOutlineThickness());

        item->setStateCallback(State::Idle, getStateCallback(State::Idle));
        item->setStateCallback(State::Hovered, getStateCallback(State::Hovered));
        item->setStateCallback(State::Selected, getStateCallback(State::Selected));
        item->getLabel().setString(name);
        item->setValue(value);
        item->setCallback([this, name](){
            setSelectedItem(name);
            getLabel().setString(name);
            close();
        });
        m_itemsContainer.addComponent(name, item);

        updateItemsPosition();
        updateScrollbarSize();
        updateScrollbarPosition();
        updateView();
    }

    template <typename T>
    void DropDownList<T>::updateView()
    {
        if (m_shape.getSize().x == 0 || m_shape.getSize().y == 0)
            return;
        m_itemsTexture.create(m_shape.getGlobalBounds().width, m_shape.getGlobalBounds().height * m_maxShownItems);
        m_itemsView.setSize(Vector2f(m_itemsTexture.getSize().x, m_itemsTexture.getSize().y));
        m_itemsView.setCenter(getPosition() + Vector2f(0, (m_shape.getGlobalBounds().height * 0.5f) +
            m_itemsTexture.getSize().y * 0.5f));
        m_itemsView.setViewport({0, 0, 1, 1});
        m_itemsTexture.setView(m_itemsView);
    }

    template <typename T>
    void DropDownList<T>::removeItem(const std::string& name)
    {
        if (!m_items.contains(name)) {
            logger::error() << "Item with name '" << name << "' does not exist.";
            return;
        }
        m_items.erase(name);
        m_orderedItems.erase(std::remove(m_orderedItems.begin(), m_orderedItems.end(), m_items[name]), m_orderedItems.end());
        m_itemsContainer.removeComponent(name);
        if (m_selectedItem == name)
            m_selectedItem = "";

        updateItemsPosition();
        updateScrollbarSize();
        updateScrollbarPosition();
        updateView();
    }

    template <typename T>
    void DropDownList<T>::open()
    {
        m_isOpen = true;
    }

    template <typename T>
    void DropDownList<T>::close()
    {
        m_isOpen = false;
    }

    template <typename T>
    void DropDownList<T>::toggle()
    {
        m_isOpen = !m_isOpen;
    }

    template <typename T>
    const DropDownItem<T>& DropDownList<T>::getSelectedItem() const
    {
        return *m_items.at(m_selectedItem);
    }

    template <typename T>
    DropDownItem<T>& DropDownList<T>::getSelectedItem()
    {
        return *m_items.at(m_selectedItem);
    }

    template <typename T>
    void DropDownList<T>::setSelectedItem(const std::string& name)
    {
        m_selectedItem = name;
        this->notify(m_items.at(name)->getValue());
    }

    template <typename T>
    void DropDownList<T>::setPlaceholder(const std::string& placeholder)
    {
        m_placeholder = placeholder;
        if (!m_selectedItem.empty() && m_items.contains(m_selectedItem) && m_selectedItem != getLabel().getString())
            getLabel().setString(m_selectedItem);
        else
            getLabel().setString(placeholder);
    }

    template <typename T>
    std::string DropDownList<T>::getPlaceholder() const
    {
        return m_placeholder;
    }

    template <typename T>
    void DropDownList<T>::setMaxShownItems(unsigned int maxShownItems)
    {
        m_maxShownItems = maxShownItems;
        updateItemsPosition();
        updateScrollbarSize();
        updateScrollbarPosition();
        updateView();
    }

    template <typename T>
    unsigned int DropDownList<T>::getMaxShownItems() const
    {
        return m_maxShownItems;
    }

    template<typename T>
    void DropDownList<T>::setSelectedItemByValue(const T& value)
    {
        for (auto& [name, item] : m_items) {
            if (item->getValue() == value) {
                setSelectedItem(name);
                getLabel().setString(name);
                return;
            }
        }
    }

    template <typename T>
    void DropDownList<T>::setPanelColor(const sf::Color& color)
    {
        m_panel.setFillColor(color);
    }
}