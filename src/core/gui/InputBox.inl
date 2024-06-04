#include "InputBox.hpp"
#include "../resource_manager/ResourceManager.hpp"
#include "helper/info/context.hpp"

namespace ypi::gui
{
    template<typename T>
    InputBox<T>::InputBox()
    {
        this->setCallback([this]() {
            m_selected = !m_selected;
        });

        if (std::is_same<T, std::wstring>::value) {
            m_autorizedChars = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789éèàçùâêîôûäëïöü-.,;:!?'\"()[]{}<>+=_@#$%&* ";
        } else if (std::is_same<T, int>::value) {
            m_autorizedChars = L"-0123456789";
        } else if (std::is_same<T, unsigned int>::value) {
            m_autorizedChars = L"0123456789";
        } else if (std::is_same<T, float>::value) {
            m_autorizedChars = L"-0123456789.,";
        }

        m_blinkTimer.setCallback([this]() {
            m_blinkState = !m_blinkState;
        });
        m_blinkTimer.setRepeat(true);
        m_blinkTimer.setTimeout(0.5f);
        m_blinkTimer.start();
    }

    template<typename T>
    InputBox<T>::InputBox(const InputBox& other)
    {
        Button::m_shape = other.m_shape;
        Button::m_text = std::make_shared<Label>(*other.m_text);
        Button::m_labelOffset = other.m_labelOffset;
        Button::m_state = other.m_state;
        Button::m_stateCallbacks = other.m_stateCallbacks;
        Button::m_callback = other.m_callback;
        m_inputString = other.m_inputString;
        m_selected = other.m_selected;

        this->setCallback([this]() {
            m_selected = !m_selected;
        });
    }

    template<typename T>
    void InputBox<T>::setCallback(std::function<void()> callback)
    {
        m_callback = callback;
    }

    template<typename T>
    void InputBox<T>::handleHoverEvents(const sf::Event &event, const Vector2f& mousePos)
    {
        Button::handleHoverEvents(event, mousePos);
    }

    template<typename T>
    void InputBox<T>::setInput(const sf::String& text, bool check)
    {
        m_inputString = text;
        if (check)
            checkInput();
        Button::getLabel().setString(m_prefix + m_inputString + m_suffix);
    }

    template<typename T>
    void InputBox<T>::setMaxChars(unsigned int maxChars)
    {
        m_maxChars = maxChars;
    }

    template<typename T>
    void InputBox<T>::setMaxDecimals(unsigned int maxDecimals)
    {
        m_maxDecimals = maxDecimals;
    }

    template<typename T>
    void InputBox<T>::checkInput()
    {

        // remove all the characters that are not in the autorizedChars string
        for (size_t i = 0; i < m_inputString.getSize(); i++) {
            if (m_autorizedChars.find(m_inputString[i]) == std::wstring::npos) {
                m_inputString.erase(i, 1);
                i--;
            }
        }

        if (std::is_same<T, int>::value) {
            size_t maxSize = m_maxChars;
            size_t signPos = m_inputString.find(L'-');
            bool hasSign = signPos != std::wstring::npos;

            if (hasSign) {
                // The sign should be at the beginning of the string.
                if (signPos != 0) {
                    m_inputString.erase(signPos, 1);
                    signPos = m_inputString.find(L'-');
                    hasSign = signPos != std::wstring::npos;
                }
                // The sign should not count towards the maxChars limit.
                maxSize += 1;
            }

            // There should be only one sign.
            if (hasSign && m_inputString.find(L'-', signPos + 1) != std::wstring::npos) {
                m_inputString.erase(m_inputString.find(L'-', signPos + 1), 1);
            }

            // Check if the input is not too big. The sign is not counted.
            if (m_inputString.getSize() > maxSize) {
                m_inputString.erase(maxSize, m_inputString.getSize() - maxSize);
            }

        } else if (std::is_same<T, float>::value) {
            size_t maxSize = m_maxChars;
            size_t signPos = m_inputString.find(L'-');
            bool hasSign = signPos != std::wstring::npos;

            //replace all the commas with dots
            size_t commaPos = m_inputString.find(L',');
            while (commaPos != std::wstring::npos) {
                m_inputString[commaPos] = L'.';
                commaPos = m_inputString.find(L',', commaPos + 1);
            }

            bool hasDot = m_inputString.find(L'.') != std::wstring::npos;

            if (hasSign)
            {
                // The sign should be at the beginning of the string.
                if (signPos != 0)
                {
                    m_inputString.erase(signPos, 1);
                    signPos = m_inputString.find(L'-');
                    hasSign = signPos != std::wstring::npos;
                }
                // The sign should not count towards the maxChars limit.
                maxSize += 1;
            }

            // There should be only one sign.
            if (hasSign && m_inputString.find(L'-', signPos + 1) != std::wstring::npos)
            {
                m_inputString.erase(m_inputString.find(L'-', signPos + 1), 1);
            }

            // There should be only one dot.
            if (hasDot && m_inputString.find(L'.', m_inputString.find(L'.') + 1) != std::wstring::npos)
            {
                m_inputString.erase(m_inputString.find(L'.', m_inputString.find(L'.') + 1), 1);
            }

            // check for the number of decimals
            if (hasDot)
            {
                size_t dotPos = m_inputString.find(L'.');
                if (m_inputString.getSize() - dotPos > m_maxDecimals + 1)
                {
                    m_inputString.erase(dotPos + m_maxDecimals + 1, m_inputString.getSize() - dotPos - m_maxDecimals - 1);
                }
                maxSize += m_maxDecimals + 1;
            }

            // Check if the input is not too big. The sign is not counted.
            if (m_inputString.getSize() > maxSize)
            {
                m_inputString.erase(maxSize, m_inputString.getSize() - maxSize);
            }
        } else if (std::is_same<T, unsigned int>::value || std::is_same<T, std::wstring>::value) {
            if (m_inputString.getSize() > m_maxChars) {
                m_inputString.erase(m_maxChars, m_inputString.getSize() - m_maxChars);
            }
        }

        if (m_cursorPosition > m_inputString.getSize())
            m_cursorPosition = m_inputString.getSize();
    }

    template<typename T>
    void InputBox<T>::handleInput(const sf::Event &event, const Vector2f& mousePos)
    {

        if (event.type == sf::Event::TextEntered && m_selected) {
            if (m_autorizedChars.find(event.text.unicode) != sf::String::InvalidPos) {
                m_inputString.insert(m_cursorPosition, event.text.unicode);
                m_cursorPosition++;
            }
            validateInput();
        } else if (event.type == sf::Event::KeyPressed && m_selected) {
            if (event.key.control && event.key.code == sf::Keyboard::C) // Ctrl+C
            {
                sf::Clipboard::setString(m_inputString.toAnsiString());
            }
            else if (event.key.control && event.key.code == sf::Keyboard::V) // Ctrl+V
            {
                m_inputString = sf::Clipboard::getString();
                validateInput();
            }
        }

    }

    template<typename T>
    void InputBox<T>::validateInput()
    {
        checkInput();
        setInput(m_inputString);
        this->notify(getValue());
    }

    template<typename T>
    void InputBox<T>::handleAnyEvents(const sf::Event &event, const Vector2f& mousePos)
    {
        Button::handleAnyEvents(event, mousePos);

        if (event.type == sf::Event::MouseButtonPressed) {
            if (!Button::getGlobalBounds().contains(mousePos)) {
                m_selected = false;
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter)
                m_selected = false;
            else if (event.key.code == sf::Keyboard::Left && m_selected) {
                if (m_cursorPosition > 0)
                    m_cursorPosition--;
            } else if (event.key.code == sf::Keyboard::Right && m_selected) {
                if (m_cursorPosition < m_inputString.getSize())
                    m_cursorPosition++;
            }
        }


        if (event.type == sf::Event::KeyPressed && m_selected) {
            if (event.key.code == sf::Keyboard::BackSpace) {
                if (m_inputString.getSize() > 0 && m_cursorPosition > 0) {
                    m_inputString.erase(m_cursorPosition - 1, 1);
                    m_cursorPosition--;
                    validateInput();
                }
            } else if (event.key.code == sf::Keyboard::Delete) {
                if (m_inputString.getSize() > 0 && m_cursorPosition < m_inputString.getSize()) {
                    m_inputString.erase(m_cursorPosition, 1);
                    validateInput();
                }
            }
        }

        handleInput(event, mousePos);
    }

    template<typename T>
    T InputBox<T>::getValue() const
    {
        T value;
        std::wstringstream ss;
        ss << m_inputString.toWideString();
        ss >> value;
        return value;
    }

    template<typename T>
    void InputBox<T>::clear()
    {
        m_inputString.clear();
        setInput(m_inputString);
    }

    template<typename T>
    void InputBox<T>::render(RenderTarget& target)
    {
        target.draw(Button::m_shape);
        if (m_selected && m_blinkState) {
            m_cursor.setSize(Vector2f(2, Button::getLabel().getCharacterSize()));
            m_cursor.setOrigin(m_cursor.getSize() / 2.f);
            m_cursor.setPosition(Button::getLabel().findCharacterPos(m_prefix.getSize() + m_cursorPosition).x, Button::getLabel().getPosition().y);
            m_cursor.setFillColor(Button::getLabel().getFillColor());
            target.draw(m_cursor);
        }
        Button::getLabel().render(target);
    }

    template<typename T>
    void InputBox<T>::setPrefix(const sf::String& prefix)
    {
        m_prefix = prefix;
        setInput(m_inputString);
    }

    template<typename T>
    void InputBox<T>::setSuffix(const sf::String& suffix)
    {
        m_suffix = suffix;
        setInput(m_inputString);
    }
}
