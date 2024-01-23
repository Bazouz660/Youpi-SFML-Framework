/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Label
*/

#ifndef EXNG_GUI_LABEL_HPP
#define EXNG_GUI_LABEL_HPP

#include "AComponent.hpp"

namespace exng::gui
{
    /**
     * @brief Classe représentant une étiquette dans l'interface graphique.
     *
     * Cette classe hérite de la classe AComponent et représente une étiquette
     * affichant du texte à l'écran.
     */
    class Label : public AComponent
    {
    private:
        sf::Text m_text;

    public:
        /**
         * @brief Constructeur par défaut de la classe Label.
         */
        Label() = default;

        /**
         * @brief Constructeur de la classe Label.
         *
         * @param position La position de l'étiquette.
         * @param text Le texte à afficher.
         * @param size La taille de la police de caractères (par défaut : 30).
         */
        Label(const exng::Vector2f &position, const std::string &text, int size = 30);

        /**
         * @brief Destructeur de la classe Label.
         */
        ~Label() override = default;

        /**
         * @brief Méthode pour afficher l'étiquette à l'écran.
         *
         * @param window La fenêtre de rendu.
         */
        void render(Window &) override;

        /**
         * @brief Méthode pour mettre à jour l'étiquette.
         */
        void update() override;

        /**
         * @brief Méthode pour gérer les événements liés à l'étiquette.
         *
         * @param event L'événement à gérer.
         */
        void handleEvent(const sf::Event &event) override;

        /**
         * @brief Méthode pour obtenir l'objet sf::Text de l'étiquette.
         *
         * @return Une référence vers l'objet sf::Text de l'étiquette.
         */
        sf::Text &getText();

        /**
         * @brief Méthode pour définir la position de l'étiquette.
         *
         * @param position La nouvelle position de l'étiquette.
         */
        virtual void setPosition(const sf::Vector2f&) override;
    };
}

#endif //EXNG_GUI_LABEL_HPP