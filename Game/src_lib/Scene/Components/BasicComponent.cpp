/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "BasicComponent.h"

namespace FA {

namespace Scene {

BasicComponent::BasicComponent(MessageBus& messageBus, const sf::IntRect& rect)
    : messageBus_(messageBus)
{
    layerTexture_.create(rect.width, rect.height);
    sprite_.setTexture(layerTexture_.getTexture());
    sprite_.setPosition(static_cast<float>(rect.left), static_cast<float>(rect.top));
}

BasicComponent::~BasicComponent() = default;

void BasicComponent::Clear()
{
    layerTexture_.clear(sf::Color::Transparent);
}

void BasicComponent::DrawTo(sf::RenderTarget& renderTarget)
{
    layerTexture_.display();
    renderTarget.draw(sprite_);
}

}  // namespace Scene

}  // namespace FA
