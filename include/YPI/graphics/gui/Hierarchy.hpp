#ifndef YPI_GUI_HIERARCHY_HPP
#define YPI_GUI_HIERARCHY_HPP

#include "InputBox.hpp"
#include "../../system/Observer.hpp"
#include "../../system/Mouse.hpp"


namespace ypi::gui
{

    // forward declaration of HierarchyFolder
    template <typename T>
    class HierarchyFolder;

    template <typename T>
    class HierarchyItem : public InputBox<std::wstring>, public std::enable_shared_from_this<HierarchyItem<T>>, public Subject<T>
    {
        public:
            HierarchyItem();
            ~HierarchyItem() override = default;

            void setValue(T value) { m_value = value; }
            T getValue() { return m_value; }

            void setParentFolder(std::shared_ptr<HierarchyFolder<T>> parentFolder);
            std::shared_ptr<HierarchyFolder<T>> getParentFolder();

            virtual void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;
            virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;

        protected:
            void validateInput() override;

        private:
            T m_value;
            std::shared_ptr<HierarchyFolder<T>> m_parentFolder = nullptr;
            MouseClickHandler m_clickHandler;
    };

    template <typename T>
    class HierarchyFolder : public HierarchyItem<T>, public Subject<bool>, public AObserver<bool>, protected AObserver<T>
    {
        protected:
            virtual void imp_setPosition() override;

        public:

            HierarchyFolder();
            ~HierarchyFolder() override = default;

            void addItem(sptr_ref<HierarchyItem<T>> item);
            void addItem(const sf::String& name, T value);

            void removeItem(std::shared_ptr<HierarchyItem<T>> item);
            void removeItem(const sf::String& name);

            void addFolder(sptr_ref<HierarchyFolder<T>> folder);
            void addFolder(const sf::String& name);

            virtual void render(RenderTarget& target) override;
            virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
            virtual void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;
            virtual FloatRect getGlobalBounds() const override;

            virtual void setLabelFont(const sf::Font& font);

            void setFolderTexture(const sf::Texture& texture);
            void setItemTexture(const sf::Texture& texture);

            virtual std::shared_ptr<HierarchyItem<T>> getItemAt(const Vector2f& mousePos, const_sptr_ref<HierarchyItem<T>> exclude = nullptr);

            void toggle();
            void expand();
            void collapse();
            bool isExpanded() const;

            std::shared_ptr<HierarchyFolder<T>> getFolder(const sf::String& name);

        protected:

            // items are always rendered in on top of the folders
            void updateItemsPositions();
            void onNotify(bool value) override;
            void onNotify(T value) override;
            void updateExpandButtonPosition();


        protected:
            void setCallback(std::function<void()> callback) override;

            std::vector<std::shared_ptr<HierarchyItem<T>>> m_items;

            Button m_expandButton;

            const sf::Texture* m_folderTexture = nullptr;
            const sf::Texture* m_itemTexture = nullptr;

            bool m_expanded = false;
    };

    template <typename T>
    class HierarchyTree : public HierarchyFolder<T>
    {
        public:
            HierarchyTree();
            ~HierarchyTree() override = default;

            virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
            virtual void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;

            std::shared_ptr<HierarchyFolder<T>> getActiveFolder();
            std::shared_ptr<HierarchyItem<T>> getSelectedItem();

            void deleteSelectedItem();

            void enableDragAndDrop();
            void disableDragAndDrop();
            bool isDragAndDropEnabled() const;

        protected:
            std::shared_ptr<HierarchyItem<T>> m_draggedItem = nullptr;
            std::shared_ptr<HierarchyItem<T>> m_targetItem = nullptr;
            std::shared_ptr<HierarchyItem<T>> m_selectedItem = nullptr;
            std::shared_ptr<HierarchyFolder<T>> m_activeFolder = nullptr;
            bool m_dragging = false;

            bool isTargetValid(const std::shared_ptr<HierarchyItem<T>>& target);

            bool m_isDragAndDropEnabled = false;
    };
}

#include "Hierarchy.inl"

#endif //YPI_GUI_HIERARCHY_HPP