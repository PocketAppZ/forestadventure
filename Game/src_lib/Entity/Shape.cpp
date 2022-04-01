/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "Shape.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Entity/EntityService.h"

namespace FA {

namespace Entity {

Shape::Shape(const EntityService *entityService)
    : entityService_(entityService)
{}

Shape::~Shape() = default;

void Shape::Register()
{
#ifdef _DEBUG
    rShape_.setSize({1.0, 1.0});
#endif
}

void Shape::Update(float deltaTime)
{
    if (currentAnimation_.IsValid()) {
        currentAnimation_.Update(deltaTime);
        currentAnimation_.ApplyTo(animationSprite_);
        animationSprite_.setPosition(entityService_->GetProperty<sf::Vector2f>("Position"));
        animationSprite_.setScale(entityService_->GetProperty<float>("Scale"),
                                  entityService_->GetProperty<float>("Scale"));
    }

    if (currentImage_.IsValid()) {
        imageSprite_.setPosition(entityService_->GetProperty<sf::Vector2f>("Position"));

        imageSprite_.setScale(entityService_->GetProperty<float>("Scale"), entityService_->GetProperty<float>("Scale"));
    }

#ifdef _DEBUG
    rShape_.setPosition(entityService_->GetProperty<sf::Vector2f>("Position"));
    rShape_.setScale(entityService_->GetProperty<float>("Scale"), entityService_->GetProperty<float>("Scale"));
#endif
}

void Shape::DrawTo(sf::RenderTarget &renderTarget)
{
    if (currentAnimation_.IsValid()) {
        renderTarget.draw(animationSprite_);
    }
    if (currentImage_.IsValid()) {
        renderTarget.draw(imageSprite_);
    }
#ifdef _DEBUG
    renderTarget.draw(rShape_);
#endif
}

void Shape::SetAnimation(const Animation &animation)
{
    if (animation.IsValid()) {
        currentAnimation_ = animation;
        currentAnimation_.ApplyTo(animationSprite_);
        currentAnimation_.Start();
    }
}

void Shape::SetImage(const Image &image)
{
    if (image.IsValid()) {
        currentImage_ = image;
        currentImage_.ApplyTo(imageSprite_);
        imageSprite_.setOrigin(imageSprite_.getLocalBounds().width / 2, imageSprite_.getLocalBounds().height / 2);
    }
}

bool Shape::AnimationIsCompleted() const
{
    return currentAnimation_.IsValid() ? currentAnimation_.IsCompleted() : true;
}

}  // namespace Entity

}  // namespace FA
