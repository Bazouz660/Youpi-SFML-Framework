/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** InputBox
*/

#ifndef EXNG_GUI_INPUTBOX_HPP
#define EXNG_GUI_INPUTBOX_HPP

#include "AComponent.hpp"
#include "Label.hpp"

namespace exng::gui
{
    /**
     * @brief Represents an input box component for GUI.
     */
    class InputBox : public AComponent
    {
    private:
        sf::Text m_input; /**< The text object used to display the input. */
        sf::String m_inputString; /**< The string representing the input. */
        enum class State {
            Idle,
            Hovered,
            Selected
        } m_state = State::Idle; /**< The state of the input box. */
        sf::Texture* m_textureIdle;
        sf::Texture* m_textureSelected;

    public:

        InputBox() = default;
        InputBox(const sf::Vector2f &size, const sf::Vector2f &position, const std::string &text);
        ~InputBox() override = default;

        void render(Window &) override;
        void update() override;
        void handleEvent(const sf::Event &event) override;

        sf::Text &getInput();
        std::string getInputString() const;
        virtual void setPosition(const sf::Vector2f&) override;
    };
}

#endif //EXNG_GUI_INPUTBOX_HPP