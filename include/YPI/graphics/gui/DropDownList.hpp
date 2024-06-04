
#pragma once

#include "Button.hpp"
#include "Container.hpp"
#include "ScrollBar.hpp"
#include "../window/RenderTarget.hpp"
#include "../window/RenderTexture.hpp"
#include "../../system/Observer.hpp"

/**
 * @brief The DropDownList class represents a drop-down list GUI component that extends the Button class.
 *
 * It allows the user to select an item from a list of options by clicking on the button and displaying a drop-down menu.
 * The selected item can be retrieved or set programmatically.
 *
 * The DropDownList class provides methods for adding and removing items, setting the maximum number of shown items,
 * customizing the appearance, and handling events.
 *
 * @note This class inherits from the Button class and overrides some of its methods.
 */
namespace ypi::gui
{

    template <typename T> // T is the type stored in the items
    class DropDownItem : public Button
    {
        public:
            DropDownItem() = default;
            ~DropDownItem() = default;

            T getValue() const { return m_value; }
            T& getValue() { return m_value; }
            void setValue(T value) { m_value = value; }

        protected:
            T m_value;
    };

    template <typename T> // T is the type stored in the items
    class DropDownList : public Button, public Subject<T>
    {
        public:
            DropDownList();
            ~DropDownList() = default;

            void render(RenderTarget& target) override;
            void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;
            void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
            FloatRect getGlobalBounds() const override;

            virtual void setBackgroundSize(const Vector2f& size) override;

            void setMaxShownItems(unsigned int maxShownItems);
            unsigned int getMaxShownItems() const;
            void setPlaceholder(const std::string& placeholder);
            std::string getPlaceholder() const;
            void addItem(const sf::String& name, T value);
            void removeItem(const std::string& name);

            void open();
            void close();
            void toggle();

            const DropDownItem<T>& getSelectedItem() const;
            DropDownItem<T>& getSelectedItem();
            void setSelectedItem(const std::string& name);

            // set the selected item by value (if 2 items have the same value, the first one is selected)
            void setSelectedItemByValue(const T& value);

            void setPanelColor(const sf::Color& color);

        protected:
            using Button::setCallback;
            void updateView();
            void updateItemsPosition();
            void updateItemsSize();
            void updateScrollbarPosition();
            void updateScrollbarSize();

            virtual void imp_setPosition() override;

        protected:
            Container m_itemsContainer;
            std::shared_ptr<ScrollBar> m_scrollBar;
            std::shared_ptr<LambdaObserver<float>> m_scrollBarObserver;
            sf::RectangleShape m_panel;
            sf::Color m_panelColor = sf::Color(10, 10, 10, 255);

            std::map<std::string, std::shared_ptr<DropDownItem<T>>> m_items;
            std::vector<std::shared_ptr<DropDownItem<T>>> m_orderedItems;
            bool m_isOpen = false;
            unsigned int m_scrollBarWidth = 5;
            unsigned int m_maxShownItems = 5;
            std::string m_selectedItem = "";
            std::string m_placeholder = "Select an item";
            Vector2f m_itemsOffset = Vector2f(0, 0);
            RenderTexture m_itemsTexture;
            sf::View m_itemsView;
            sf::RectangleShape m_itemsSprite;
    };
}

#include "DropDownList.inl"
