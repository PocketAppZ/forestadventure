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
    virtual void HandleIsKeyPressed(Keyboard::Key key) override;
    virtual void HandleIsKeyReleased(Keyboard::Key key) override;
    virtual void HandleKeyPressed(Keyboard::Key key) override;
    virtual std::vector<MessageType> Messages() const override;

private:
    virtual void AddAttributes(EntityService& handler, const AttributeData& data) override;
    virtual void RegisterModes(ModeController& modeController, const EntityService& entityService) override;
};

}  // namespace Entity

}  // namespace FA
