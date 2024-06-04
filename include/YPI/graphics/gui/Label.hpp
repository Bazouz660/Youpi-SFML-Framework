/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Label
*/

#ifndef YPI_GUI_LABEL_HPP
#define YPI_GUI_LABEL_HPP

#include "AComponent.hpp"

namespace ypi::gui
{
    /**
     * @brief Classe représentant une étiquette dans l'interface graphique.
     *
     * Cette classe hérite de la classe AComponent et représente une étiquette
     * affichant du texte à l'écran.
     */
    class Label : virtual public AComponent, public AObserver<const std::string&>
    {
    public:
        enum class AlignmentH {
            LEFT,
            CENTER,
            RIGHT
        };

        enum class AlignmentV {
            TOP,
            CENTER,
            BOTTOM
        };

    protected:
        sf::Text m_text;
        AlignmentH m_alignmentH = AlignmentH::LEFT;
        AlignmentV m_alignmentV = AlignmentV::BOTTOM;

        virtual void imp_setPosition() override;

    public:
        /**
         * @brief Constructeur par défaut de la classe Label.
         */
        Label() = default;
        Label(const Label&);

        /**
         * @brief Destructeur de la classe Label.
         */
        ~Label() override = default;

        void setString(const sf::String& text);
        void setFont(const sf::Font& font);
        void setCharacterSize(unsigned int size);
        void setFillColor(const sf::Color& color);
        void setOutlineColor(const sf::Color& color);
        void setOutlineThickness(float thickness);
        void setHorizontalAlignment(AlignmentH alignment);
        void setVerticalAlignment(AlignmentV alignment);
        void setStyle(sf::Text::Style style);

        Vector2f findCharacterPos(std::size_t index) const;

        sf::String getString() const;
        const sf::Font& getFont() const;
        unsigned int getCharacterSize() const;
        const sf::Color& getFillColor() const;
        const sf::Color& getOutlineColor() const;
        float getOutlineThickness() const;
        AlignmentH getHorizontalAlignment() const;
        AlignmentV getVerticalAlignment() const;
        Vector2f getOrigin() const;

        /**
         * @brief Méthode pour afficher l'étiquette à l'écran.
         *
         * @param window La fenêtre de rendu.
         */
        void render(RenderTarget &target) override;

        /**
         * @brief Méthode pour obtenir les limites de l'étiquette.
         *
         * @return Les limites de l'étiquette.
         */
        virtual FloatRect getGlobalBounds() const override;

        private:
            void onNotify(const std::string& event) override;
            void align();
    };
}

#endif //YPI_GUI_LABEL_HPP