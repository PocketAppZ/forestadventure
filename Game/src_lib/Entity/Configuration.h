/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <SFML/System/Vector2.hpp>

#include "Enum/AnimationType.h"
#include "Enum/EntityType.h"
#include "Enum/FaceDirection.h"
#include "Enum/TileType.h"

namespace FA {

namespace Entity {

struct Configuration
{
    EntityType entityType_{};
    sf::Vector2u position_{};
    FaceDirection faceDir_{};
    float velocity_{};
    TileType tileType_{};
};

}  // namespace Entity

}  // namespace FA