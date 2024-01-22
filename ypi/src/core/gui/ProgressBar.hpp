/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** ProgressBar
*/

#ifndef EXNG_GUI_PROGRESSBAR_HPP
#define EXNG_GUI_PROGRESSBAR_HPP

#include "AComponent.hpp"

namespace exng::gui
{
    /**
     * @brief Classe représentant une barre de progression dans l'interface graphique.
     */
    class ProgressBar : public AComponent
    {
    private:
        float m_progress = 0; /**< La valeur de progression de la barre. */
        sf::RectangleShape m_bar; /**< La forme rectangulaire de la barre. */

    public:
        /**
         * @brief Constructeur de la classe ProgressBar.
         * @param size La taille de la barre.
         * @param position La position de la barre.
         */
        ProgressBar(const sf::Vector2f& size, const sf::Vector2f& position);

        /**
         * @brief Destructeur par défaut de la classe ProgressBar.
         */
        ~ProgressBar() override = default;

        /**
         * @brief Obtient la valeur de progression de la barre.
         * @return La valeur de progression de la barre.
         */
        float getProgress() const;

        /**
         * @brief Définit la valeur de progression de la barre.
         * @param progress La nouvelle valeur de progression.
         */
        void setProgress(float progress);

        /**
         * @brief Affiche la barre de progression sur une fenêtre.
         * @param window La fenêtre sur laquelle afficher la barre.
         */
        void render(Window& window) override;

        /**
         * @brief Met à jour la barre de progression.
         */
        void update() override;

        /**
         * @brief Gère les événements liés à la barre de progression.
         * @param event L'événement à gérer.
         */
        void handleEvent(const sf::Event& event) override;

        /**
         * @brief Obtient la forme rectangulaire de la barre.
         * @return La forme rectangulaire de la barre.
         */
        sf::RectangleShape& getBar();

        /**
         * @brief Définit la position de la barre.
         * @param position La nouvelle position de la barre.
         */
        virtual void setPosition(const sf::Vector2f& position) override;
    };
}

#endif //EXNG_GUI_PROGRESSBAR_HPP
