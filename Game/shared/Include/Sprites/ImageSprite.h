/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <memory>

#include "BasicSprite.h"

#include "Resource/Frame.h"

namespace FA {

namespace Shared {

class ImageSprite : public BasicSprite
{
public:
    ImageSprite(std::shared_ptr<Graphic::ISprite> sprite, const Frame &frame);
    ImageSprite() = default;

    virtual void Update(float deltaTime) override;

private:
    Frame frame_;
};

}  // namespace Shared

}  // namespace FA