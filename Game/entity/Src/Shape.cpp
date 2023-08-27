/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "Shape.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Body.h"

namespace FA {

namespace Entity {

Shape::Shape(Body& body)
    : body_(body)
{
#ifdef _DEBUG
    rShape_.setSize({1.0, 1.0});
#endif
}

void Shape::Enter()
{
    for (auto part : parts_) {
        part->Enter();
        part->SetPosition(body_.position_);
        part->SetRotation(body_.rotation_);
    }
}

void Shape::Update(float deltaTime)
{
    for (auto part : parts_) {
        part->Update(deltaTime);
        part->SetPosition(body_.position_);
        part->SetRotation(body_.rotation_);
    }
}

void Shape::RegisterPart(std::shared_ptr<BasicShapePart> part)
{
    parts_.push_back(part);
}

void Shape::DrawTo(sf::RenderTarget& renderTarget)
{
    for (auto part : parts_) {
        part->DrawTo(renderTarget);
    }
#ifdef _DEBUG
    renderTarget.draw(rShape_);
#endif
}

}  // namespace Entity

}  // namespace FA
