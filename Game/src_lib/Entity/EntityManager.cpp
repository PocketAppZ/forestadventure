/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "EntityManager.h"

#include "Entity/Entities/BasicEntity.h"
#include "Factory.h"
#include "Logging.h"

namespace FA {

namespace Entity {

EntityManager::EntityManager(const Factory& factory)
    : factory_(factory)
{}

EntityManager::~EntityManager()
{
    for (const auto& entry : entityMap_) {
        entry.second->OnDestroy();
    }
}

void EntityManager::DrawTo(sf::RenderTarget& renderTarget) const
{
    for (const auto& entry : entityMap_) {
        entry.second->DrawTo(renderTarget);
    }
}

void EntityManager::Update(float deltaTime)
{
    for (const auto& entry : entityMap_) {
        entry.second->Update(deltaTime);
    }
}

void EntityManager::LateUpdate()
{
    for (const auto& entry : entityMap_) {
        entry.second->LateUpdate();
    }
}

BasicEntity* EntityManager::Create(EntityType type, const ComponentHandler& componentHandler)
{
    auto entity = factory_.Create(type, componentHandler);
    auto id = entity->GetId();
    entity->OnCreate();
    AddEntity(std::move(entity));

    return entityMap_.at(id).get();
}

void EntityManager::Init(const AnimationDb& animationDb)
{
    for (const auto& entry : entityMap_) {
        entry.second->OnInit(animationDb);
    }
}

void EntityManager::AddEntity(std::unique_ptr<Entity::BasicEntity> entity)
{
    auto id = entity->GetId();
    if (entityMap_.find(id) == entityMap_.end()) {
        entityMap_[id] = std::move(entity);
    }
    else {
        LOG_ERROR("id: ", id, " already exist");
    }
}

void EntityManager::EnableInput(bool enable)
{
    for (const auto& entry : entityMap_) {
        entry.second->EnableInput(enable);
    }
}

}  // namespace Entity

}  // namespace FA
