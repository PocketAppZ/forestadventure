/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicEntity.h"

namespace FA {

namespace Entity {

class ArrowEntity : public BasicEntity
{
public:
    ArrowEntity(EntityId id, CameraManager& cameraManager, const SheetManager& sheetManager,
                EntityManager& entityManager, MessageBus& messageBus);
    virtual ~ArrowEntity();

    virtual std::string Name() const override { return "ArrowEntity"; }
    virtual EntityType Type() const override { return EntityType::Arrow; }

protected:
    virtual std::vector<ImageData> Images() const override;

    virtual void PostUpdate(EntityService& entityService) override;

private:
    virtual void DefineProperties(EntityService& handler, const PropertyData& data) override;
    virtual void DefineModes(ModeController& modeController) override;
    virtual void DefineShape(EntityService& entityService, Shape& shape) override;
};

}  // namespace Entity

}  // namespace FA