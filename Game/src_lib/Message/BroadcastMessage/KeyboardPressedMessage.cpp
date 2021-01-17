/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "KeyboardPressedMessage.h"

namespace FA {

KeyboardPressedMessage::KeyboardPressedMessage(Keyboard::Key key)
    : key_(key)
{}

}  // namespace FA
