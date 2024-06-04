#include "AComponent.hpp"

#pragma once

namespace ypi::gui
{
    class Panel : virtual public AComponent
    {
    public:
        Panel();
        ~Panel() = default;

        virtual void render(RenderTarget& target) override;

    protected:
        virtual void imp_setPosition() override;
    };
}
