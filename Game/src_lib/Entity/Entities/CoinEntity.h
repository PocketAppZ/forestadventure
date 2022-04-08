/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicEntity.h"

#include "Resource/Animation.h"

namespace FA {

namespace Entity {

class CoinEntity : public BasicEntity
{
public:
    CoinEntity(EntityId id, CameraManager& cameraManager, const SheetManager& sheetManager,
               EntityManager& entityManager, MessageBus& messageBus);
    virtual ~CoinEntity();

    virtual std::string Name() const override { return "CoinEntity"; }
    virtual EntityType Type() const override { return EntityType::Coin; }

private:
    Animation animation_;

private:
    virtual void RegisterModes(ModeController& modeController) override;
    virtual void RegisterProperties(EntityService& entityService) override;
    virtual void InitModes(const ModeController& modeController, const EntityService& entityService,
                           const PropertyData& data) override;

    void BuildAnimations(const EntityService& entityService, ModeType modeType);
    Animation GetAnimation(const EntityService& entityService, ModeType modeType) const;
};

}  // namespace Entity

}  // namespace FA
