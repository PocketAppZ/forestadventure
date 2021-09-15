/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "CameraManager.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Camera.h"

namespace FA {

CameraManager::CameraManager(const sf::Vector2u& renderTargetSize, const sf::Vector2u& mapSize)
    : renderTargetSize_(renderTargetSize)
    , mapSize_(mapSize)
{}

CameraManager::~CameraManager() = default;

void CameraManager::Track(const sf::Vector2f& position)
{
    camera_ = std::make_unique<Camera>(position, renderTargetSize_, mapSize_);
}

void CameraManager::Update(sf::RenderTarget& renderTarget)
{
    if (camera_) camera_->UpdatePosition(renderTarget);
}

}  // namespace FA
