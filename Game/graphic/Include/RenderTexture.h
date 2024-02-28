/*
 *	Copyright (C) 2023 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <memory>

#include "IRenderTexture.h"
#include "SfmlFwd.h"

namespace FA {

namespace Graphic {

class RenderTexture : public IRenderTexture
{
public:
    RenderTexture();
    virtual ~RenderTexture();

    virtual void draw(const IDrawable& drawable) override;
    virtual bool create(unsigned int width, unsigned int height, bool depthBuffer = false) override;
    virtual void display() override;
    virtual sf::Vector2u getSize() const override;
    virtual const Graphic::ITexture& getTexture() const override;
    virtual void clear() override;
    virtual void clear(const sf::Color& color) override;
    virtual void setView(const Graphic::IView& view) override;
    virtual sf::Vector2f mapPixelToCoords(const sf::Vector2i& point) const override;

private:
    std::unique_ptr<sf::RenderTexture> renderTexture_;
    std::shared_ptr<const Graphic::ITexture> texture_;
};

}  // namespace Graphic

}  // namespace FA
