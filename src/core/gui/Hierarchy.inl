#include "Hierarchy.hpp"
#include "helper/info/logger.hpp"

namespace ypi::gui
{
    template <typename T>
    HierarchyItem<T>::HierarchyItem()
    {
        setCallback([this]() {
            this->Subject<T>::notify(this->getValue());
        });
        //this->getLabel().setHorizontalAlignment(Label::AlignmentH::LEFT);
        //this->getLabel().setVerticalAlignment(Label::AlignmentV::CENTER);
    }

    template <typename T>
    HierarchyFolder<T>::HierarchyFolder()
    {
        this->m_expandButton.setBackgroundSize({20, 20});
        this->m_expandButton.setBackgroundColor(sf::Color::Transparent);
        this->m_expandButton.getLabel().setString(">");
        this->m_expandButton.getLabel().setCharacterSize(20);
        this->m_expandButton.getLabel().setFillColor(sf::Color::White);
        this->m_expandButton.setCallback([this]() {
            this->toggle();
            this->m_expandButton.getLabel().setString(m_expanded ? "^" : ">");
        });

        this->m_expandButton.setStateCallback(Button::State::Idle, [](Button* button) {
            button->getLabel().setFillColor(sf::Color(175, 175, 175, 255));
            button->getLabel().setOutlineColor(sf::Color::White);
        });
        this->m_expandButton.setStateCallback(Button::State::Hovered, [](Button* button) {
            button->getLabel().setFillColor(sf::Color(230, 230, 230, 255));
            button->getLabel().setOutlineColor(sf::Color::White);
        });
        this->m_expandButton.setStateCallback(Button::State::Selected, [](Button* button) {
            button->getLabel().setFillColor(sf::Color(150, 150, 150, 255));
            button->getLabel().setOutlineColor(sf::Color::White);
        });

        updateExpandButtonPosition();
    }

    template <typename T>
    void HierarchyFolder<T>::updateExpandButtonPosition()
    {
        FloatRect bounds = InputBox<std::wstring>::getGlobalBounds();
        this->m_expandButton.setPosition({bounds.left + bounds.width + this->m_expandButton.getShape().getGlobalBounds().width * 0.5f ,  bounds.top + this->m_expandButton.getShape().getGlobalBounds().height * 0.5f});
    }

    template <typename T>
    void HierarchyFolder<T>::onNotify(bool value)
    {
        Subject<bool>::notify(value);
        this->updateItemsPositions();
    }

    template <typename T>
    void HierarchyFolder<T>::removeItem(std::shared_ptr<HierarchyItem<T>> item)
    {
        this->removeChild(item);
        m_items.erase(std::remove(m_items.begin(), m_items.end(), item), m_items.end());
        Subject<bool>::notify(m_expanded);
        this->updateItemsPositions();
    }

    template <typename T>
    void HierarchyFolder<T>::removeItem(const sf::String& name)
    {
        for (auto& item : m_items) {
            if (item->getLabel().getString() == name) {
                m_items.erase(std::remove(m_items.begin(), m_items.end(), item), m_items.end());
                this->updateItemsPositions();
                return;
            }
        }
    }

    template <typename T>
    void HierarchyFolder<T>::addItem(sptr_ref<HierarchyItem<T>> item)
    {
        m_items.push_back(item);
        if (item->getParentFolder()) {
            item->Subject<T>::removeObserver(item->getParentFolder().get());
            item->getParentFolder()->removeItem(item);
        }
        item->setParentFolder(std::dynamic_pointer_cast<HierarchyFolder<T>>(this->shared_from_this()));
        item->Subject<T>::addObserver(this);
        this->addChild(item);
        this->updateItemsPositions();
        Subject<bool>::notify(m_expanded);
    }

    template <typename T>
    void HierarchyFolder<T>::addFolder(sptr_ref<HierarchyFolder<T>> folder)
    {
        m_items.push_back(folder);
        if (folder->getParentFolder()) {
            folder->Subject<T>::removeObserver(folder->getParentFolder().get());
            folder->Subject<bool>::removeObserver(folder->getParentFolder().get());
            folder->getParentFolder()->removeItem(folder);
        }
        folder->setParentFolder(std::dynamic_pointer_cast<HierarchyFolder<T>>(this->shared_from_this()));
        this->addChild(folder);
        folder->Subject<bool>::addObserver(this);
        folder->Subject<T>::addObserver(this);
        this->updateItemsPositions();
        Subject<bool>::notify(m_expanded);
    }

    template <typename T>
    void HierarchyFolder<T>::render(RenderTarget& target)
    {
        HierarchyItem<T>::render(target);
        if (m_expanded) {
            for (auto& item : m_items) {
                item->render(target);
            }
        }

        this->m_expandButton.render(target);
    }

    template <typename T>
    void HierarchyItem<T>::handleHoverEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        Button::handleHoverEvents(event, mousePos);

        if (HierarchyItem<T>::getGlobalBounds().contains(mousePos) && m_clickHandler.isDoubleClick(event)) {
            m_selected = !m_selected;
        }
    }

    template <typename T>
    void HierarchyItem<T>::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        InputBox<std::wstring>::handleAnyEvents(event, mousePos);
        //getLabel().setOffset(Vector2f(this->getShape().getGlobalBounds().width + getLabel().getGlobalBounds().width / 2.f, this->m_labelOffset.y));
    }

    template <typename T>
    void HierarchyFolder<T>::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        this->m_expandButton.handleAnyEvents(event, mousePos);
        HierarchyItem<T>::handleAnyEvents(event, mousePos);

        if (m_expanded) {
            for (auto& item : m_items) {
                item->handleAnyEvents(event, mousePos);
            }
        }

        updateExpandButtonPosition();
    }

    template <typename T>
    void HierarchyFolder<T>::handleHoverEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        if (this->m_expandButton.getGlobalBounds().contains(mousePos)) {
            this->m_expandButton.handleHoverEvents(event, mousePos);
            return;
        }

        HierarchyItem<T>::handleHoverEvents(event, mousePos);

        if (m_expanded) {
            for (auto& item : m_items) {
                item->handleHoverEvents(event, mousePos);
            }
        }
    }

    template <typename T>
    void HierarchyFolder<T>::setCallback(std::function<void()> callback)
    {
        this->m_callback = callback;
    }

    template <typename T>
    FloatRect HierarchyFolder<T>::getGlobalBounds() const
    {
        FloatRect bounds = Button::getGlobalBounds();
        bounds = bounds.combine(this->m_expandButton.getGlobalBounds());

        if (m_expanded) {
            for (auto& item : m_items) {
                bounds = bounds.combine(item->getGlobalBounds());
            }
        }
        return bounds;
    }

    template <typename T>
    void HierarchyFolder<T>::updateItemsPositions()
    {

        // udpate items

        float y = this->m_shape.getSize().y;
        float x = this->m_shape.getSize().x;
        for (auto& item : m_items) {
            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (folder) continue;
            item->setPosition({x, y});
            y += item->getGlobalBounds().height;
        }

        for (auto& item : m_items) {
            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (folder) {
                folder->setPosition({x, y});
                y += folder->getGlobalBounds().height;
            }
        }

        updateExpandButtonPosition();
   }

    template <typename T>
    void HierarchyFolder<T>::addFolder(const sf::String& name)
    {
        auto folder = std::make_shared<HierarchyFolder<T>>();
        folder->m_shape = this->m_shape;
        if (m_itemTexture) {
            folder->m_itemTexture = m_itemTexture;
        }
        folder->getLabel().setFont(this->getLabel().getFont());
        folder->getLabel().setCharacterSize(this->getLabel().getCharacterSize());
        folder->getLabel().setFillColor(this->getLabel().getFillColor());
        folder->getLabel().setOutlineColor(this->getLabel().getOutlineColor());
        folder->getLabel().setOutlineThickness(this->getLabel().getOutlineThickness());
        folder->getLabel().setHorizontalAlignment(this->getLabel().getHorizontalAlignment());
        folder->getLabel().setVerticalAlignment(this->getLabel().getVerticalAlignment());

        folder->setLabelFont(this->getLabel().getFont());

        folder->setInput(name);
        folder->setLabelOffset({folder->getShape().getGlobalBounds().width, 0.f});
        folder->m_items.clear();
        folder->m_expandButton.setCallback([folder]() {
            folder->toggle();
            folder->m_expandButton.getLabel().setString(folder->m_expanded ? "^" : ">");
        });
        folder->setStateCallback(Button::State::Idle, this->getStateCallback(Button::State::Idle));
        folder->setStateCallback(Button::State::Hovered, this->getStateCallback(Button::State::Hovered));
        folder->setStateCallback(Button::State::Selected, this->getStateCallback(Button::State::Selected));
        folder->setIdle();

        addFolder(folder);
    }

    template <typename T>
    void HierarchyFolder<T>::setLabelFont(const sf::Font& font)
    {
        Button::getLabel().setFont(font);
        for (auto& item : m_items) {
            item->getLabel().setFont(font);
        }
        this->m_expandButton.getLabel().setFont(font);
    }

    template <typename T>
    void HierarchyFolder<T>::addItem(const sf::String& name, T value)
    {
        auto item = std::make_shared<HierarchyItem<T>>();
        item->setBackgroundSize(this->m_shape.getSize());
        if (m_itemTexture) {
            item->setBackgroundTexture(*m_itemTexture);
        }
        item->getLabel().setFont(this->getLabel().getFont());
        item->getLabel().setCharacterSize(this->getLabel().getCharacterSize());
        item->getLabel().setFillColor(this->getLabel().getFillColor());
        item->getLabel().setOutlineColor(this->getLabel().getOutlineColor());
        item->getLabel().setOutlineThickness(this->getLabel().getOutlineThickness());
        item->getLabel().setHorizontalAlignment(this->getLabel().getHorizontalAlignment());
        item->getLabel().setVerticalAlignment(this->getLabel().getVerticalAlignment());
        item->setInput(name);
        item->setLabelOffset({item->getShape().getGlobalBounds().width, 0.f});
        item->setValue(value);
        addItem(item);
    }

    template <typename T>
    void HierarchyFolder<T>::imp_setPosition()
    {
        Button::imp_setPosition();
        this->updateItemsPositions();
    }

    template <typename T>
    void HierarchyFolder<T>::toggle()
    {
        m_expanded = !m_expanded;
        Subject<bool>::notify(m_expanded);
    }

    template <typename T>
    void HierarchyFolder<T>::expand()
    {
        m_expanded = true;
        Subject<bool>::notify(m_expanded);
    }

    template <typename T>
    void HierarchyFolder<T>::collapse()
    {
        m_expanded = false;
        Subject<bool>::notify(m_expanded);
    }

    template <typename T>
    void HierarchyFolder<T>::setFolderTexture(const sf::Texture& texture)
    {
        this->m_shape.setTexture(&texture);
        m_folderTexture = &texture;
        for (auto& item : m_items) {
            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (!folder) continue;
            folder->setFolderTexture(texture);
        }
    }

    template <typename T>
    void HierarchyFolder<T>::setItemTexture(const sf::Texture& texture)
    {
        for (auto& item : m_items) {
            item->setBackgroundTexture(texture);
        }
        for (auto& item : m_items) {
            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (!folder) continue;
            folder->setItemTexture(texture);
        }
        m_itemTexture = &texture;
    }

    template <typename T>
    std::shared_ptr<HierarchyItem<T>> HierarchyFolder<T>::getItemAt(const Vector2f& mousePos, const_sptr_ref<HierarchyItem<T>> exclude)
    {
        if (!getGlobalBounds().contains(mousePos)) return nullptr;
        if (!m_expanded && !HierarchyItem<T>::getGlobalBounds().contains(mousePos)) return nullptr;

        for (auto& item : m_items) {
            if (item == exclude) continue;
            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (folder) {
                if (folder->HierarchyItem<T>::getGlobalBounds().contains(mousePos)) {
                    return folder;
                }
                if (folder->m_expanded) {
                    auto result = folder->getItemAt(mousePos, exclude);
                    if (result) return result;
                }
            } else if (item->getGlobalBounds().contains(mousePos)) {
                return item;
            }
        }
        return nullptr;
    }

    template <typename T>
    bool HierarchyFolder<T>::isExpanded() const
    {
        return m_expanded;
    }

    template <typename T>
    void HierarchyItem<T>::setParentFolder(std::shared_ptr<HierarchyFolder<T>> parentFolder)
    {
        m_parentFolder = parentFolder;
    }

    template <typename T>
    std::shared_ptr<HierarchyFolder<T>> HierarchyItem<T>::getParentFolder()
    {
        return m_parentFolder;
    }

    template <typename T>
    std::shared_ptr<HierarchyFolder<T>> HierarchyFolder<T>::getFolder(const sf::String& name)
    {
        for (auto& item : m_items) {
            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (!folder) continue;
            if (folder->m_text.getString() == name) {
                return folder;
            }
        }
        throw std::runtime_error("Hierarchy: folder " + name + " not found");
    }

    template <typename T>
    void HierarchyTree<T>::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        HierarchyFolder<T>::handleAnyEvents(event, mousePos);

        if (!m_isDragAndDropEnabled) return;
        if (event.type == sf::Event::MouseMoved && m_targetItem && !m_dragging) {
            m_targetItem->setBackgroundOutlineThickness(0);
            m_targetItem = nullptr;
        }
    }

    template <typename T>
    void HierarchyTree<T>::handleHoverEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        HierarchyFolder<T>::handleHoverEvents(event, mousePos);

        if (!m_isDragAndDropEnabled) return;

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (HierarchyItem<T>::getGlobalBounds().contains(mousePos)) {
                m_activeFolder = std::dynamic_pointer_cast<HierarchyFolder<T>>(this->shared_from_this());
                return;
            }
            auto item = this->getItemAt(mousePos, m_draggedItem);
            if (!item) return;
            m_dragging = true;
            m_draggedItem = item;
            m_selectedItem = item;

            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (folder) {
                m_activeFolder = folder;
            } else {
                m_activeFolder = item->getParentFolder();
            }
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && m_dragging) {
            m_dragging = false;

            std::shared_ptr<HierarchyItem<T>> item = nullptr;
            if (HierarchyItem<T>::getGlobalBounds().contains(mousePos)) {
                item = this->shared_from_this();
            } else
                item = this->getItemAt(mousePos, m_draggedItem);

            if (!item) {
                m_draggedItem = nullptr;
                this->updateItemsPositions();
                return;
            }

            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (isTargetValid(folder)) {
                auto folderItem = std::dynamic_pointer_cast<HierarchyFolder<T>>(m_draggedItem);
                if (folderItem) {
                    folder->addFolder(folderItem);
                } else
                    folder->addItem(m_draggedItem);
                folder->expand();
            }
            m_draggedItem = nullptr;
            this->updateItemsPositions();
        }

        if (event.type == sf::Event::MouseMoved && m_dragging) {
            m_draggedItem->setAbsolutePosition(mousePos);

            std::shared_ptr<HierarchyItem<T>> item = nullptr;
            if (HierarchyItem<T>::getGlobalBounds().contains(mousePos)) {
                item = this->shared_from_this();
            } else
                item = this->getItemAt(mousePos, m_draggedItem);

            if (!item) return;

            auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(item);
            if (isTargetValid(folder)) {
                if (m_targetItem)
                    m_targetItem->setBackgroundOutlineThickness(0);
                m_targetItem = folder;
                folder->setBackgroundOutlineColor(sf::Color::Green);
                folder->setBackgroundOutlineThickness(1);
            }
        }
    }

    template <typename T>
    void HierarchyItem<T>::validateInput()
    {
        InputBox<std::wstring>::validateInput();
        this->Subject<T>::notify(this->getValue());
    }

    template <typename T>
    HierarchyTree<T>::HierarchyTree()
    {
        this->setCallback([this]() {
        });
    }

    template <typename T>
    bool HierarchyTree<T>::isTargetValid(const std::shared_ptr<HierarchyItem<T>>& target)
    {
        bool valid = false;
        if (target && m_draggedItem != target && m_draggedItem->getParentFolder() != target) {
            valid = true;
        }
        auto folder = std::dynamic_pointer_cast<HierarchyFolder<T>>(target);
        if (!folder) return valid;

        // check if we are not dragging a parent folder into a child folder
        auto parent = folder->getParentFolder();
        while (parent) {
            if (parent == m_draggedItem) {
                valid = false;
                break;
            }
            parent = parent->getParentFolder();
        }
        return valid;
    }

    template <typename T>
    std::shared_ptr<HierarchyFolder<T>> HierarchyTree<T>::getActiveFolder()
    {
        if (!m_activeFolder) {
            m_activeFolder = std::dynamic_pointer_cast<HierarchyFolder<T>>(this->shared_from_this());
        }
        return m_activeFolder;
    }

    template <typename T>
    std::shared_ptr<HierarchyItem<T>> HierarchyTree<T>::getSelectedItem()
    {
        return m_selectedItem;
    }

    template <typename T>
    void HierarchyTree<T>::enableDragAndDrop()
    {
        m_isDragAndDropEnabled = true;
    }

    template <typename T>
    void HierarchyTree<T>::disableDragAndDrop()
    {
        m_isDragAndDropEnabled = false;
    }

    template <typename T>
    bool HierarchyTree<T>::isDragAndDropEnabled() const
    {
        return m_isDragAndDropEnabled;
    }

    template <typename T>
    void HierarchyTree<T>::deleteSelectedItem()
    {
        if (m_selectedItem) {
            m_selectedItem->getParentFolder()->removeItem(m_selectedItem);
            m_selectedItem = nullptr;
            m_draggedItem = nullptr;
        }
    }

    template <typename T>
    void HierarchyFolder<T>::onNotify(T value)
    {
        Subject<T>::notify(value);
    }

} // namespace ypi::gui
