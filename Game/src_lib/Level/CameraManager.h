/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <memory>

#include <SFML/System/Vector2.hpp>

#include "Fwd/SfmlFwd.h"

namespace FA {

class Camera;

class CameraManager
{
public:
    CameraManager(sf::RenderTarget& renderTarget, const sf::Vector2u& mapSize);
    ~CameraManager();

    void Track(const sf::Vector2f& position);
    void Update();

private:
    sf::RenderTarget& renderTarget_;
    sf::Vector2u mapSize_{};
    std::unique_ptr<Camera> camera_ = nullptr;
};

}  // namespace FA
