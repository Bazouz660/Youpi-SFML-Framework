#ifndef YPI_GUI_ACOMPONENT_HPP
#define YPI_GUI_ACOMPONENT_HPP

#include "../../../lib_headers/common.hpp"
#include "../window/Window.hpp"
#include "../../math/Rect.hpp"
#include "../../system/Observer.hpp"
#include "../../system/logger.hpp"

namespace ypi::gui
{
    /**
     * @brief The base class for GUI components.
     *
     * This class provides a common interface and functionality for GUI components.
     * It contains a protected member variable `m_shape` of type `sf::RectangleShape`
     * which represents the shape of the component.
     */
    class AComponent
    {
    protected:
        sf::RectangleShape m_shape;
        AComponent* m_parent = nullptr;
        std::set<std::shared_ptr<AComponent>> m_children;
        Vector2f m_absolutePosition = {0, 0};
        Vector2f m_relativePosition = {0, 0};

    public:
        /**
         * @brief Default constructor.
         */
        AComponent() = default;

        /**
         * @brief Virtual destructor.
         */
        virtual ~AComponent() = default;

        /**
         * @brief Get the shape of the component.
         *
         * @return A reference to the `sf::RectangleShape` representing the shape of the component.
         */
        sf::RectangleShape& getShape() { return m_shape; };
        const sf::RectangleShape& getShape() const { return m_shape; };
        void setShape(const sf::RectangleShape& shape) { m_shape = shape; }

        /**
         * @brief Render the component on a window.
         *
         * @param window The window on which to render the component.
         */
        virtual void render(RenderTarget&) = 0;

        /**
         * @brief Set the position of the component.
         *
         * @param pos The new position of the component.
         */
        virtual void setPosition(const Vector2f& pos) final {
            m_relativePosition = pos;
            updatePosition(false);
        }

        virtual void setAbsolutePosition(const Vector2f& pos) final {
            m_absolutePosition = pos;
            updatePosition(true);
        }

        virtual Vector2f getRelativePosition() const final {
            return m_relativePosition;
        }

        virtual AComponent* getParent() const final {
            return m_parent;
        }

        virtual void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) {}
        virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) {}

        /**
         * @brief Get the position of the component.
         *
         * @return The position of the component.
         */
        virtual Vector2f getPosition() const final {
            return m_absolutePosition;
        }

        virtual FloatRect getGlobalBounds() const {
            return m_shape.getGlobalBounds();
        }

        virtual FloatRect getLocalBounds() const final {
            FloatRect bounds = m_shape.getLocalBounds();
            bounds.left = m_relativePosition.x - m_shape.getOrigin().x;
            bounds.top = m_relativePosition.y - m_shape.getOrigin().y;
            return bounds;
        }

        virtual void addChild(const_sptr_ref<AComponent> child) final {
            m_children.insert(child);
            child->setParent(this);
            child->updatePosition(false);
            updatePosition(false);
        }

        virtual void removeChild(const_sptr_ref<AComponent> child) final {
            child->setParent(nullptr);
            m_children.erase(child);
            child->updatePosition(false);
            updatePosition(false);
        }

        virtual void setParent(AComponent* parent) final {
            m_parent = parent;
            updatePosition(false);
        }

        protected:

            // This function is called to update the position of the elements of the component. It is meant to be overridden by derived classes.
            virtual void imp_setPosition() {
                m_shape.setPosition(m_absolutePosition);
            }

            virtual void updatePosition(bool absolute) final {
                if (!absolute) {
                    if (m_parent) {
                        m_absolutePosition = m_parent->getPosition() + m_relativePosition;
                    } else {
                        m_absolutePosition = m_relativePosition;
                    }
                }
                imp_setPosition();

                for (auto& child : m_children) {
                    child->updatePosition(false);
                }
            }
    };
}

#endif //YPI_GUI_ACOMPONENT_HPP