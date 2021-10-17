/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "BasicAttribute.h"

namespace FA {

namespace Entity {

BasicAttribute::BasicAttribute(EntityService *owner)
    : BasicProperty(owner)
{}

BasicAttribute::~BasicAttribute() = default;

}  // namespace Entity

}  // namespace FA
