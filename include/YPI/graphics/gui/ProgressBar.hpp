/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** ProgressBar
*/

#ifndef YPI_GUI_PROGRESSBAR_HPP
#define YPI_GUI_PROGRESSBAR_HPP

#include "AComponent.hpp"

namespace ypi::gui
{
    /**
     * @brief Classe représentant une barre de progression dans l'interface graphique.
     */
    class ProgressBar : public AComponent, public AObserver<float>
    {
    public:
        enum class Alignment {
            HORIZONTAL,
            VERTICAL
        };

    protected:
        float m_progress = 0; /**< La valeur de progression de la barre. */
        sf::RectangleShape m_bar; /**< La forme rectangulaire de la barre. */
        Alignment m_alignment = Alignment::HORIZONTAL;

        virtual void imp_setPosition() override;

    public:

        /**
         * @brief Constructeur de la classe ProgressBar.
         * @param size La taille de la barre.
         * @param position La position de la barre.
         */
        ProgressBar();

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

        void setSize(const Vector2f& size);
        void setAlignment(Alignment alignment);

        /**
         * @brief Affiche la barre de progression sur une fenêtre.
         * @param window La fenêtre sur laquelle afficher la barre.
         */
        void render(RenderTarget& target) override;

        /**
         * @brief Obtient la forme rectangulaire de la barre.
         * @return La forme rectangulaire de la barre.
         */
        sf::RectangleShape& getBar();

        virtual FloatRect getGlobalBounds() const override;

        private:
            void onNotify(float data) override;
    };
}

#endif //YPI_GUI_PROGRESSBAR_HPP
