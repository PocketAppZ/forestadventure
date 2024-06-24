/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "MoleEntity.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Abilities/MoveAbility.h"
#include "Constant/Entity.h"
#include "Events/CollisionEvent.h"
#include "Events/DeadEvent.h"
#include "PropertyConverter.h"
#include "PropertyData.h"
#include "Resource/SheetId.h"
#include "Resource/SheetItem.h"
#include "ShapeParts/AnimationPart.h"
#include "ShapeParts/ColliderPart.h"
#include "State.h"

namespace FA {

namespace Entity {

namespace {

using namespace Shared::SheetId;

const Shared::SheetItem idleSide1{MoleIdleSide, {0, 0}};
const Shared::SheetItem idleFront1{MoleIdleFront, {0, 0}};
const Shared::SheetItem idleBack1{MoleIdleBack, {0, 0}};

const Shared::SheetItem moveSide1{MoleWalkSide, {0, 0}};
const Shared::SheetItem moveSide2{MoleWalkSide, {1, 0}};
const Shared::SheetItem moveSide3{MoleWalkSide, {2, 0}};
const Shared::SheetItem moveSide4{MoleWalkSide, {3, 0}};

const Shared::SheetItem moveDown1{MoleWalkFront, {0, 0}};
const Shared::SheetItem moveDown2{MoleWalkFront, {1, 0}};
const Shared::SheetItem moveDown3{MoleWalkFront, {2, 0}};
const Shared::SheetItem moveDown4{MoleWalkFront, {3, 0}};

const Shared::SheetItem moveUp1{MoleWalkBack, {0, 0}};
const Shared::SheetItem moveUp2{MoleWalkBack, {1, 0}};
const Shared::SheetItem moveUp3{MoleWalkBack, {2, 0}};
const Shared::SheetItem moveUp4{MoleWalkBack, {3, 0}};

const Shared::SheetItem collision1{Death, {0, 0}};
const Shared::SheetItem collision2{Death, {1, 0}};
const Shared::SheetItem collision3{Death, {2, 0}};
const Shared::SheetItem collision4{Death, {3, 0}};
const Shared::SheetItem collision5{Death, {4, 0}};
const Shared::SheetItem collision6{Death, {5, 0}};

const std::vector<Shared::ImageData> idleLeft{{idleSide1, true}};
const std::vector<Shared::ImageData> idleRight{idleSide1};
const std::vector<Shared::ImageData> idleFront{idleFront1};
const std::vector<Shared::ImageData> idleBack{idleBack1};
const std::vector<Shared::ImageData> moveLeft{
    {moveSide1, true}, {moveSide2, true}, {moveSide3, true}, {moveSide4, true}};
const std::vector<Shared::ImageData> moveRight{moveSide1, moveSide2, moveSide3, moveSide4};
const std::vector<Shared::ImageData> moveDown{moveDown1, moveDown2, moveDown3, moveDown4};
const std::vector<Shared::ImageData> moveUp{moveUp1, moveUp2, moveUp3, moveUp4};

const std::unordered_map<FaceDirection, std::vector<Shared::ImageData>> idleData{{FaceDirection::Left, idleLeft},
                                                                                 {FaceDirection::Right, idleRight},
                                                                                 {FaceDirection::Down, idleFront},
                                                                                 {FaceDirection::Up, idleBack}};
const std::unordered_map<FaceDirection, std::vector<Shared::ImageData>> moveData{{FaceDirection::Left, moveLeft},
                                                                                 {FaceDirection::Right, moveRight},
                                                                                 {FaceDirection::Down, moveDown},
                                                                                 {FaceDirection::Up, moveUp}};

const std::vector<Shared::ImageData> collision{collision1, collision2, collision3, collision4, collision5, collision6};

const std::vector<Shared::ColliderData> colliderIdleLeft{idleSide1};
const std::vector<Shared::ColliderData> colliderIdleRight{idleSide1};
const std::vector<Shared::ColliderData> colliderIdleFront{idleFront1};
const std::vector<Shared::ColliderData> colliderIdleBack{idleBack1};
const std::vector<Shared::ColliderData> colliderMoveLeft{moveSide1, moveSide2, moveSide3, moveSide4};
const std::vector<Shared::ColliderData> colliderMoveRight{moveSide1, moveSide2, moveSide3, moveSide4};
const std::vector<Shared::ColliderData> colliderMoveDown{moveDown1, moveDown2, moveDown3, moveDown4};
const std::vector<Shared::ColliderData> colliderMoveUp{moveUp1, moveUp2, moveUp3, moveUp4};

const std::unordered_map<FaceDirection, std::vector<Shared::ColliderData>> colliderIdleData{
    {FaceDirection::Left, colliderIdleLeft},
    {FaceDirection::Right, colliderIdleRight},
    {FaceDirection::Down, colliderIdleFront},
    {FaceDirection::Up, colliderIdleBack}};
const std::unordered_map<FaceDirection, std::vector<Shared::ColliderData>> colliderMoveData{
    {FaceDirection::Left, colliderMoveLeft},
    {FaceDirection::Right, colliderMoveRight},
    {FaceDirection::Down, colliderMoveDown},
    {FaceDirection::Up, colliderMoveUp}};

FaceDirection MoveDirToFaceDir(MoveDirection moveDirection)
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

    return faceDir;
}

}  // namespace

const std::string MoleEntity::str = "Mole";

MoleEntity::MoleEntity(EntityId id, const PropertyData& data, const Shared::MapData& mapData,
                       const EntityService& service)
    : BasicEntity(id, data, mapData, service)
{}

MoleEntity::~MoleEntity() = default;

void MoleEntity::OnBeginMove(MoveDirection moveDirection)
{
    FaceDirection faceDir = MoveDirToFaceDir(moveDirection);
    propertyStore_.Set("FaceDirection", faceDir);
}

void MoleEntity::OnUpdateMove(const sf::Vector2f& delta)
{
    body_.position_ += delta;
}

void MoleEntity::RegisterProperties()
{
    propertyStore_.Register("FaceDirection", FaceDirection::Down);
}

void MoleEntity::ReadProperties(const std::unordered_map<std::string, std::string>& properties)
{
    for (const auto& p : properties) {
        if (p.first == "FaceDirection") {
            FaceDirection dir = ToValue<FaceDirection>(p.second);
            propertyStore_.Set(p.first, dir);
        }
    }
}

void MoleEntity::RegisterStates(std::shared_ptr<State> idleState, std::shared_ptr<State> deadState,
                                const PropertyData& data)
{
    DefineIdleState(idleState);
    auto moveState = RegisterState(StateType::Move);
    DefineMoveState(moveState);
    auto collisionState = RegisterState(StateType::Collision);
    DefineCollisionState(collisionState);
}

void MoleEntity::DefineIdleState(std::shared_ptr<State> state)
{
    auto shapePart = MakeShapePart(idleData);
    state->RegisterShapePart(shapePart);
    auto colliderPart = MakeColliderPart(colliderIdleData);
    state->RegisterColliderPart(colliderPart);

    state->RegisterEventCB(EventType::StartMove,
                           [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Move, event); });
    state->RegisterEventCB(EventType::Collision, [this](std::shared_ptr<BasicEvent> event) {
        auto collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(event);
        if (service_.GetType(collisionEvent->id_) == EntityType::Arrow) {
            ChangeStateTo(StateType::Collision, event);
        }
    });
    state->RegisterIgnoreEvents({EventType::StopMove});
}

void MoleEntity::DefineMoveState(std::shared_ptr<State> state)
{
    auto shapePart = MakeShapePart(moveData);
    state->RegisterShapePart(shapePart);
    auto colliderPart = MakeColliderPart(colliderMoveData);
    state->RegisterColliderPart(colliderPart);
    auto move = std::make_shared<MoveAbility>(
        Constant::stdVelocity, [this](MoveDirection d) { OnBeginMove(d); },
        [this](const sf::Vector2f& d) { OnUpdateMove(d); });
    state->RegisterAbility(move);
    state->RegisterEventCB(EventType::StopMove,
                           [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Idle, event); });
    state->RegisterEventCB(EventType::Collision, [this](std::shared_ptr<BasicEvent> event) {
        auto collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(event);
        if (service_.GetType(collisionEvent->id_) == EntityType::Arrow) {
            ChangeStateTo(StateType::Collision, event);
        }
    });
}

void MoleEntity::DefineCollisionState(std::shared_ptr<State> state)
{
    auto updateCB = [this](const Shared::ImageAnimation& animation) {
        if (animation.IsCompleted()) {
            HandleEvent(std::make_shared<DeadEvent>());
        }
    };
    auto animation = service_.MakeAnimation(collision);
    auto shapePart = AnimationPart::Create(animation);
    shapePart->RegisterUpdateCB(updateCB);
    state->RegisterShapePart(shapePart);
    state->RegisterIgnoreEvents({EventType::Collision});
}

std::shared_ptr<AnimationPartWith<FaceDirection>> MoleEntity::MakeShapePart(
    const std::unordered_map<FaceDirection, std::vector<Shared::ImageData>>& data)
{
    FaceDirection* dir = nullptr;
    propertyStore_.GetPtr<FaceDirection>("FaceDirection", dir);
    auto part = AnimationPartWith<FaceDirection>::Create(*dir);
    for (const auto& entry : data) {
        part->RegisterAnimation(entry.first, service_.MakeAnimation(entry.second));
    }

    return part;
}

std::shared_ptr<ColliderPartWith<FaceDirection>> MoleEntity::MakeColliderPart(
    const std::unordered_map<FaceDirection, std::vector<Shared::ColliderData>>& data)
{
    FaceDirection* dir = nullptr;
    propertyStore_.GetPtr<FaceDirection>("FaceDirection", dir);
    auto part = ColliderPartWith<FaceDirection>::Create(*dir);
    for (const auto& entry : data) {
        part->RegisterCollider(entry.first, service_.MakeAnimation(entry.second));
    }

    return part;
}

}  // namespace Entity

}  // namespace FA
