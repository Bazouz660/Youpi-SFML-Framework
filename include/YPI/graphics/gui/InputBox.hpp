/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** InputBox
*/

#ifndef YPI_GUI_INPUTBOX_HPP
#define YPI_GUI_INPUTBOX_HPP

#include "Button.hpp"
#include "Label.hpp"
#include "../../system/Observer.hpp"
#include "../../system/Timer.hpp"

namespace ypi::gui
{
    /**
     * @brief Represents an input box component for GUI.
     */

    template<typename T>
    class InputBox : public Button, public Subject<T>
    {
    protected:
        sf::String m_inputString; /**< The string representing the input. */
        bool m_selected = false; /**< The state of the input box. */

    public:

        // static assert to check if T is wstring, int, float, unsigned int
        static_assert(std::is_same<T, std::wstring>::value || std::is_same<T, int>::value || std::is_same<T, float>::value || std::is_same<T, unsigned int>::value, "InputBox can only be of type std::string, std::wstring, int, float, unsigned int");

        InputBox();
        InputBox(const InputBox&);
        ~InputBox() override = default;

        virtual void handleHoverEvents(const sf::Event &event, const Vector2f& mousePos) override;
        virtual void handleAnyEvents(const sf::Event &event, const Vector2f& mousePos) override;
        virtual void render(RenderTarget& target) override;

        void setInput(const sf::String& text, bool check = false);
        void checkInput();

        void setMaxChars(unsigned int maxChars);
        void setMaxDecimals(unsigned int maxDecimals);

        void setPrefix(const sf::String& prefix);
        void setSuffix(const sf::String& suffix);
        sf::String getPrefix() const;
        sf::String getSuffix() const;

        void clear();

        T getValue() const;

    protected:
        void setCallback(std::function<void()> callback) override;
        sf::RectangleShape m_cursor;
        unsigned int m_maxChars = 12;
        unsigned int m_maxDecimals = 2;

        Vector2f m_inputViewSize;

        sf::String m_prefix = "";
        sf::String m_suffix = "";
        std::wstring m_autorizedChars;
        AsyncTimer m_blinkTimer;
        std::atomic<bool> m_blinkState = false;

        void handleInput(const sf::Event &event, const Vector2f& mousePos);
        virtual void validateInput();

    private:
        size_t m_cursorPosition = 0;
    };

}

#include "InputBox.inl"

#endif //YPI_GUI_INPUTBOX_HPP