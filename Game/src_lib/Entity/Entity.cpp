/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "Entity.h"

#include "Constant/Screen.h"
#include "Game/Layer.h"
#include "Message/BroadcastMessage/IsKeyPressedMessage.h"
#include "Message/MessageBus.h"
#include "Misc/TextureManager.h"
#include "Sprite/SpriteSheet.h"

namespace FA {

Entity::Entity(MessageBus& messageBus, TextureManager& textureManager)
    : messageBus_(messageBus)
    , animationHandler_(0.1f)
{
    messageBus_.AddSubscriber("entity", MessageType::IsKeyPressed,
                              [this](std::shared_ptr<Message> message) { OnMessage(message); });
    rectShape_.setPosition(constant::Screen::centerX_f, constant::Screen::centerY_f);
    constexpr int size = 64;
    rectShape_.setSize({static_cast<float>(size), static_cast<float>(size)});
    InitAnimation(textureManager);
    animationHandler_.SetAnimation(frameType_, dir_, rectShape_, true);
}

void Entity::Update(float deltaTime)
{
    animationHandler_.Update(deltaTime);
}

void Entity::DrawTo(Layer& layer)
{
    layer.Draw(rectShape_);
}

void Entity::OnMessage(std::shared_ptr<Message> msg)
{
    if (msg->GetMessageType() == MessageType::IsKeyPressed) {
        // TODO: Implement a mechanism to disable handling of IsKeyPressed when game is paused.
        auto m = std::dynamic_pointer_cast<IsKeyPressedMessage>(msg);
        auto key = m->GetKey();
        OnIsKeyPressed(key);
    }
}

void Entity::SetFrameType(AnimationHandler::FrameType frameType)
{
    frameType_ = frameType;
    animationHandler_.SetAnimation(frameType_, dir_, rectShape_, true);
}

void Entity::SetFaceDir(AnimationHandler::FaceDir dir)
{
    dir_ = dir;
    animationHandler_.SetAnimation(frameType_, dir_, rectShape_, true);
}

void Entity::OnIsKeyPressed(Keyboard::Key key)
{
    if (key == Keyboard::Key::Right) {
        SetFaceDir(AnimationHandler::FaceDir::Right);
    }
    else if (key == Keyboard::Key::Left) {
        SetFaceDir(AnimationHandler::FaceDir::Left);
    }
    else if (key == Keyboard::Key::Up) {
        SetFaceDir(AnimationHandler::FaceDir::Up);
    }
    else if (key == Keyboard::Key::Down) {
        SetFaceDir(AnimationHandler::FaceDir::Down);
    }
    else if (key == Keyboard::Key::Num1) {
        SetFrameType(AnimationHandler::FrameType::Idle);
    }
    else if (key == Keyboard::Key::Num2) {
        SetFrameType(AnimationHandler::FrameType::Move);
    }
    else if (key == Keyboard::Key::Num3) {
        SetFrameType(AnimationHandler::FrameType::Attack);
    }
}

void Entity::InitAnimation(TextureManager& textureManager)
{
    auto textureWalkSide =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/walk/hero-walk-side.png");
    if (textureWalkSide != nullptr) {
        SpriteSheet spriteSheet("heroWalkSide", textureWalkSide, {6, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 6, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Move, AnimationHandler::FaceDir::Left,
                                                f.texture_, f.frames_, f.defaultFrame_, true);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Move, AnimationHandler::FaceDir::Right,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }
    auto textureWalkFront =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/walk/hero-walk-front.png");
    if (textureWalkFront != nullptr) {
        SpriteSheet spriteSheet("heroWalkFront", textureWalkFront, {6, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 6, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Move, AnimationHandler::FaceDir::Down,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }
    auto textureWalkBack =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/walk/hero-back-walk.png");
    if (textureWalkBack != nullptr) {
        SpriteSheet spriteSheet("heroWalkBack", textureWalkBack, {6, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 6, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Move, AnimationHandler::FaceDir::Up,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }

    auto textureAttackSide =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/attack/hero-attack-side.png");
    if (textureAttackSide != nullptr) {
        SpriteSheet spriteSheet("heroAttackSide", textureAttackSide, {3, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 3, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Attack, AnimationHandler::FaceDir::Left,
                                                f.texture_, f.frames_, f.defaultFrame_, true);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Attack, AnimationHandler::FaceDir::Right,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }
    auto textureAttackFront =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/attack/hero-attack-front.png");
    if (textureAttackFront != nullptr) {
        SpriteSheet spriteSheet("heroAttackFront", textureAttackFront, {3, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 3, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Attack, AnimationHandler::FaceDir::Down,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }
    auto textureAttackBack =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/attack/hero-attack-back.png");
    if (textureAttackBack != nullptr) {
        SpriteSheet spriteSheet("heroAttackBack", textureAttackBack, {3, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 3, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Attack, AnimationHandler::FaceDir::Up,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }

    auto textureIdleSide =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/idle/hero-idle-side.png");
    if (textureIdleSide != nullptr) {
        SpriteSheet spriteSheet("heroIdleSide", textureIdleSide, {1, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 1, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Idle, AnimationHandler::FaceDir::Left,
                                                f.texture_, f.frames_, f.defaultFrame_, true);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Idle, AnimationHandler::FaceDir::Right,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }
    auto textureIdleFront =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/idle/hero-idle-front.png");
    if (textureIdleFront != nullptr) {
        SpriteSheet spriteSheet("heroIdleFront", textureIdleFront, {1, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 1, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Idle, AnimationHandler::FaceDir::Down,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }
    auto textureIdleBack =
        textureManager.GetTexture("assets/tiny-RPG-forest-files/PNG/spritesheets/hero/idle/hero-idle-back.png");
    if (textureIdleBack != nullptr) {
        SpriteSheet spriteSheet("heroIdleBack", textureIdleBack, {1, 1});
        SpriteSheet::FrameData f = spriteSheet.Scan({0, 0}, 1, 0);
        animationHandler_.RegisterAnimationInfo(AnimationHandler::FrameType::Idle, AnimationHandler::FaceDir::Up,
                                                f.texture_, f.frames_, f.defaultFrame_);
    }
}

}  // namespace FA