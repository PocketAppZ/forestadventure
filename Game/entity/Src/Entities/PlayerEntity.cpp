/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "PlayerEntity.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Abilities/MoveAbility.h"
#include "Camera.h"
#include "Constant/Entity.h"
#include "Entities/ArrowEntity.h"
#include "Events/AttackEvent.h"
#include "Events/AttackWeaponEvent.h"
#include "Events/DeadEvent.h"
#include "Events/StartMoveEvent.h"
#include "Events/StopMoveEvent.h"
#include "Message/BroadcastMessage/GameOverMessage.h"
#include "Message/BroadcastMessage/IsKeyPressedMessage.h"
#include "Message/BroadcastMessage/KeyPressedMessage.h"
#include "Message/BroadcastMessage/KeyReleasedMessage.h"
#include "Message/MessageType.h"
#include "PropertyConverter.h"
#include "PropertyData.h"
#include "Resource/AnimationData.h"
#include "Resource/SheetId.h"
#include "Sprites/AnimationSprite.h"
#include "State.h"

namespace FA {

namespace Entity {

namespace {

const std::unordered_map<FaceDirection, sf::Vector2f> arrowOffset = {{FaceDirection::Down, {0.0, 15.0}},
                                                                     {FaceDirection::Left, {-15.0, 5.0}},
                                                                     {FaceDirection::Right, {15.0, 5.0}},
                                                                     {FaceDirection::Up, {0.0, -15.0}}};

const std::unordered_map<FaceDirection, Shared::AnimationData> idleData{
    {FaceDirection::Left, {Shared::SheetId::HeroIdleSide, {{0, 0}, 1, 0}, true}},
    {FaceDirection::Right, {Shared::SheetId::HeroIdleSide, {{0, 0}, 1, 0}, false}},
    {FaceDirection::Down, {Shared::SheetId::HeroIdleFront, {{0, 0}, 1, 0}, false}},
    {FaceDirection::Up, {Shared::SheetId::HeroIdleBack, {{0, 0}, 1, 0}, false}}};

const std::unordered_map<FaceDirection, Shared::AnimationData> moveData{
    {FaceDirection::Left, {Shared::SheetId::HeroWalkSide, {{0, 0}, 6, 0}, true}},
    {FaceDirection::Right, {Shared::SheetId::HeroWalkSide, {{0, 0}, 6, 0}, false}},
    {FaceDirection::Down, {Shared::SheetId::HeroWalkFront, {{0, 0}, 6, 0}, false}},
    {FaceDirection::Up, {Shared::SheetId::HeroWalkBack, {{0, 0}, 6, 0}, false}}};

const std::unordered_map<FaceDirection, Shared::AnimationData> attackData{
    {FaceDirection::Left, {Shared::SheetId::HeroAttackSide, {{0, 0}, 3, 0}, true}},
    {FaceDirection::Right, {Shared::SheetId::HeroAttackSide, {{0, 0}, 3, 0}, false}},
    {FaceDirection::Down, {Shared::SheetId::HeroAttackFront, {{0, 0}, 3, 0}, false}},
    {FaceDirection::Up, {Shared::SheetId::HeroAttackBack, {{0, 0}, 3, 0}, false}}};

const std::unordered_map<FaceDirection, Shared::AnimationData> attackWData{
    {FaceDirection::Left, {Shared::SheetId::HeroAttackWeaponSide, {{0, 0}, 3, 0}, true}},
    {FaceDirection::Right, {Shared::SheetId::HeroAttackWeaponSide, {{0, 0}, 3, 0}, false}},
    {FaceDirection::Down, {Shared::SheetId::HeroAttackWeaponFront, {{0, 0}, 3, 0}, false}},
    {FaceDirection::Up, {Shared::SheetId::HeroAttackWeaponBack, {{0, 0}, 3, 0}, false}}};

}  // namespace

const std::string PlayerEntity::str = "Player";

PlayerEntity::PlayerEntity(EntityId id, const PropertyData& data, const EntityService& service)
    : BasicEntity(id, data, service)
{}

PlayerEntity::~PlayerEntity() = default;

std::vector<Shared::MessageType> PlayerEntity::Messages() const
{
    return {Shared::MessageType::IsKeyPressed, Shared::MessageType::KeyReleased, Shared::MessageType::KeyPressed};
}

void PlayerEntity::OnMessage(std::shared_ptr<Shared::Message> msg)
{
    if (msg->GetMessageType() == Shared::MessageType::IsKeyPressed) {
        auto m = std::dynamic_pointer_cast<Shared::IsKeyPressedMessage>(msg);
        auto key = m->GetKey();
        if (key == sf::Keyboard::Key::Right) {
            HandleEvent(std::make_shared<StartMoveEvent>(MoveDirection::Right));
        }
        else if (key == sf::Keyboard::Key::Left) {
            HandleEvent(std::make_shared<StartMoveEvent>(MoveDirection::Left));
        }
        else if (key == sf::Keyboard::Key::Down) {
            HandleEvent(std::make_shared<StartMoveEvent>(MoveDirection::Down));
        }
        else if (key == sf::Keyboard::Key::Up) {
            HandleEvent(std::make_shared<StartMoveEvent>(MoveDirection::Up));
        }
        else if (key == sf::Keyboard::Key::RControl) {
            HandleEvent(std::make_shared<AttackEvent>());
        }
        else if (key == sf::Keyboard::Key::Space) {
            HandleEvent(std::make_shared<AttackWeaponEvent>());
        }
    }
    else if (msg->GetMessageType() == Shared::MessageType::KeyReleased) {
        auto m = std::dynamic_pointer_cast<Shared::KeyReleasedMessage>(msg);
        auto key = m->GetKey();
        if (key == sf::Keyboard::Key::Right || key == sf::Keyboard::Key::Left || key == sf::Keyboard::Key::Down ||
            key == sf::Keyboard::Key::Up) {
            HandleEvent(std::make_shared<StopMoveEvent>());
        }
    }
    else if (msg->GetMessageType() == Shared::MessageType::KeyPressed) {
        auto m = std::dynamic_pointer_cast<Shared::KeyPressedMessage>(msg);
        auto key = m->GetKey();
        if (key == sf::Keyboard::Key::Num1) {
            HandleEvent(std::make_shared<DeadEvent>());
        }
    }
}

void PlayerEntity::OnBeginMove(MoveDirection moveDirection)
{
    FaceDirection faceDir = FaceDirection::Undefined;
    if (moveDirection == MoveDirection::Down)
        faceDir = FaceDirection::Down;
    else if (moveDirection == MoveDirection::Up)
        faceDir = FaceDirection::Up;
    else if (moveDirection == MoveDirection::Left)
        faceDir = FaceDirection::Left;
    else if (moveDirection == MoveDirection::Right)
        faceDir = FaceDirection::Right;
    propertyStore_.Set("FaceDirection", faceDir);
}

void PlayerEntity::OnUpdateMove(const sf::Vector2f& delta)
{
    body_.position_ += delta;
}

void PlayerEntity::OnShoot()
{
    bool shoot = propertyStore_.Get<bool>("Shoot");
    if (shoot) {
        propertyStore_.Set<bool>("Shoot", false);
        auto dir = propertyStore_.Get<FaceDirection>("FaceDirection");
        auto position = body_.position_ + arrowOffset.at(dir);
        MoveDirection moveDir = MoveDirection::None;
        if (dir == FaceDirection::Down)
            moveDir = MoveDirection::Down;
        else if (dir == FaceDirection::Up)
            moveDir = MoveDirection::Up;
        else if (dir == FaceDirection::Left)
            moveDir = MoveDirection::Left;
        else if (dir == FaceDirection::Right)
            moveDir = MoveDirection::Right;
        auto data = ArrowEntity::CreatePropertyData(position, moveDir);
        entityService_.CreateEntity(data);
    }
}

void PlayerEntity::OnBeginDie()
{
    SendMessage(std::make_shared<Shared::GameOverMessage>());
    auto& camera = entityService_.GetCamera();
    camera.Set(body_.position_);
}

void PlayerEntity::RegisterProperties()
{
    propertyStore_.Register<FaceDirection>("FaceDirection", FaceDirection::Down);
    propertyStore_.Register<bool>("Shoot", false);
}

void PlayerEntity::ReadProperties(const std::unordered_map<std::string, std::string>& properties)
{
    for (const auto& p : properties) {
        if (p.first == "FaceDirection") {
            FaceDirection dir = ToValue<FaceDirection>(p.second);
            propertyStore_.Set<FaceDirection>(p.first, dir);
        }
    }
}

void PlayerEntity::RegisterStates(std::shared_ptr<State> idleState, std::shared_ptr<State> deadState,
                                  const PropertyData& data)
{
    RegisterIdleState(idleState);
    RegisterMoveState();
    RegisterAttackState();
    RegisterAttackWeaponState();
}

void PlayerEntity::OnInit()
{
    auto& camera = entityService_.GetCamera();
    camera.Track(body_.position_);
}

void PlayerEntity::RegisterIdleState(std::shared_ptr<State> idleState)
{
    auto sprite = std::make_shared<AnimationSprite<FaceDirection>>(
        [this]() { return propertyStore_.Get<FaceDirection>("FaceDirection"); });
    for (const auto& entry : idleData) {
        sprite->RegisterResource(entry.first, entityService_.MakeAnimation(entry.second));
    }
    idleState->RegisterSprite(sprite);
    idleState->RegisterEventCB(EventType::StartMove,
                               [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Move, event); });
    idleState->RegisterEventCB(EventType::StopMove, [this](std::shared_ptr<BasicEvent> event) {});
    idleState->RegisterEventCB(EventType::Attack,
                               [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Attack, event); });
    idleState->RegisterEventCB(EventType::AttackWeapon, [this](std::shared_ptr<BasicEvent> event) {
        ChangeStateTo(StateType::AttackWeapon, event);
    });
    idleState->RegisterIgnoreEvents({EventType::Collision});
}

void PlayerEntity::RegisterMoveState()
{
    auto moveState = RegisterState(StateType::Move);
    auto move = std::make_shared<MoveAbility>(
        Constant::stdVelocity, [this](MoveDirection d) { OnBeginMove(d); },
        [this](const sf::Vector2f& d) { OnUpdateMove(d); });
    auto sprite = std::make_shared<AnimationSprite<FaceDirection>>(
        [this]() { return propertyStore_.Get<FaceDirection>("FaceDirection"); });
    for (const auto& entry : moveData) {
        sprite->RegisterResource(entry.first, entityService_.MakeAnimation(entry.second));
    }
    moveState->RegisterAbility(move);
    moveState->RegisterSprite(sprite);
    moveState->RegisterEventCB(EventType::StopMove,
                               [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Idle, event); });
    moveState->RegisterIgnoreEvents({EventType::StartMove, EventType::Attack, EventType::AttackWeapon});
}

void PlayerEntity::RegisterAttackState()
{
    auto attackState = RegisterState(StateType::Attack);
    auto attackAnimationUpdateCB = [this](const Shared::Animation& animation) {
        if (animation.IsCompleted()) {
            ChangeStateTo(StateType::Idle, nullptr);
        }
    };
    auto sprite = std::make_shared<AnimationSprite<FaceDirection>>(
        [this]() { return propertyStore_.Get<FaceDirection>("FaceDirection"); }, attackAnimationUpdateCB);
    for (const auto& entry : attackData) {
        sprite->RegisterResource(entry.first, entityService_.MakeAnimation(entry.second));
    }
    attackState->RegisterSprite(sprite);
    attackState->RegisterEventCB(EventType::StartMove,
                                 [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Move, event); });
    attackState->RegisterIgnoreEvents({EventType::Attack, EventType::AttackWeapon});
}

void PlayerEntity::RegisterAttackWeaponState()
{
    auto attackWeaponState = RegisterState(StateType::AttackWeapon);
    attackWeaponState->RegisterExitCB([this]() { OnShoot(); });
    auto attackWeaponAnimationUpdateCB = [this](const Shared::Animation& animation) {
        if (animation.IsCompleted()) {
            propertyStore_.Set<bool>("Shoot", true);
            ChangeStateTo(StateType::Idle, nullptr);
        }
    };
    auto sprite = std::make_shared<AnimationSprite<FaceDirection>>(
        [this]() { return propertyStore_.Get<FaceDirection>("FaceDirection"); }, attackWeaponAnimationUpdateCB);
    for (const auto& entry : attackWData) {
        sprite->RegisterResource(entry.first, entityService_.MakeAnimation(entry.second));
    }
    attackWeaponState->RegisterSprite(sprite);
    attackWeaponState->RegisterEventCB(
        EventType::StartMove, [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Move, event); });
    attackWeaponState->RegisterIgnoreEvents({EventType::Attack, EventType::AttackWeapon});
}

}  // namespace Entity

}  // namespace FA
