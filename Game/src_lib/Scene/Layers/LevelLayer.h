/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <memory>

#include "BasicLayer.h"

#include "Level/CameraManager.h"
#include "Resource/TextureManager.h"

namespace FA {

class BasicEffect;
class Level;

namespace Scene {

class LevelLayer : public BasicLayer
{
public:
    LevelLayer(MessageBus& messageBus, const sf::IntRect& rect, TextureManager& textureManager);
    virtual ~LevelLayer();

    virtual LayerId GetId() const override { return LayerId::Level; }
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
    virtual void EnableInput(bool enable) override;
    virtual void EnterTransition(const BasicTransition& transition) override;
    virtual void ExitTransition(const BasicTransition& transition) override;
    virtual void OnCreate() override;

private:
    std::unique_ptr<Level> level_ = nullptr;
    std::unique_ptr<BasicEffect> effect_ = nullptr;
    CameraManager cameraManager_;
};

}  // namespace Scene

}  // namespace FA