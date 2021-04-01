/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicState.h"

namespace FA {

namespace Entity {

class UninitializedState : public BasicState
{
public:
    UninitializedState(StateMachine& stateMachine, StateData& stateData);
    virtual ~UninitializedState();

    virtual void Update(float deltaTime) override {}
    virtual void DrawTo(sf::RenderTarget& renderTarget) override {}
    virtual std::string Name() const override { return "UninitializedState"; }

    virtual void OnInitStateData(FaceDirection faceDir, float velocity, std::unique_ptr<BasicSprite> sprite) override;
};

}  // namespace Entity

}  // namespace FA
