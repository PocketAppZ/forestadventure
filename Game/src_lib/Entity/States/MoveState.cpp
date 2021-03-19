/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "MoveState.h"

#include "Entity/Components/Sprite/BasicSprite.h"
#include "Enum/FrameType.h"
#include "IdleState.h"

namespace FA {

namespace Entity {

MoveState::MoveState(StateMachine& stateMachine, StateData& stateData)
    : BasicState(stateMachine, stateData)
    , movement_(stateData_.velocity_)
{}

MoveState::~MoveState() = default;

void MoveState::Update(float deltaTime)
{
    stateData_.sprite_->Update(deltaTime);
    movement_.Update(deltaTime);
    stateData_.sprite_->Move(movement_);
}

void MoveState::DrawTo(sf::RenderTarget& renderTarget)
{
    stateData_.sprite_->DrawTo(renderTarget);
}

void MoveState::Enter()
{
    movement_.SetDirection(stateData_.moveDir_);
    stateData_.sprite_->SetAnimation(FrameType::Move, stateData_.faceDir_);
    stateData_.sprite_->StartAnimation();
}

void MoveState::Exit()
{
    movement_.SetDirection(MoveDirection::None);
}

void MoveState::OnStopMove()
{
    SwitchState<IdleState>();
}

}  // namespace Entity

}  // namespace FA
