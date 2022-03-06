/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicEntity.h"

namespace FA {

namespace Entity {

class PlayerEntity : public BasicEntity
{
public:
    PlayerEntity(EntityId id, CameraManager& cameraManager, const SheetManager& sheetManager,
                 EntityManager& entityManager, MessageBus& messageBus);
    virtual ~PlayerEntity();

    virtual std::string Name() const override { return "PlayerEntity"; }
    virtual EntityType Type() const override { return EntityType::Player; }

protected:
    virtual std::vector<MessageType> Messages() const override;

private:
    virtual void RegisterModes(ModeController& modeController) override;
    virtual void RegisterAttributes(EntityService& entityService) override;
    virtual void InitModes(const ModeController& modeController, const EntityService& entityService,
                           const AttributeData& data) override;
    virtual void InitAttributes(EntityService& entityServcice, const AttributeData& data) override;
    virtual void OnMessage(std::shared_ptr<Message> msg) override;
};

}  // namespace Entity

}  // namespace FA
