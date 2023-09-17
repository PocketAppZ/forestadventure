/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <unordered_set>

#include <SFML/Window/Keyboard.hpp>

#include "IRenderWindow.h"

namespace FA {

namespace Shared {

class MessageBus;

}  // namespace Shared

class InputSystem
{
public:
    InputSystem(Shared::MessageBus& messageBus, Graphic::IRenderWindow& window);

    void Update(float deltaTime);

private:
    Graphic::IRenderWindow& window_;
    Shared::MessageBus& messageBus_;
    std::unordered_set<sf::Keyboard::Key> pressedKeys_;

private:
    void ProcessEvent(const sf::Event& event);
    void ProcessIsKeyPressed();
    void ReleaseKeys();
};

}  // namespace FA
