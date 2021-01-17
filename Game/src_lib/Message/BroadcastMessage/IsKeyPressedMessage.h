/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "Message/Message.h"

#include "Enum/KeyboardKey.h"

namespace FA {

class IsKeyPressedMessage : public Message
{
public:
    IsKeyPressedMessage(Keyboard::Key key);

    virtual MessageType GetMessageType() const override { return MessageType::IsKeyPressed; }
    virtual std::string Name() const override { return "IsKeyPressedMessage"; }

    Keyboard::Key GetKey() const { return key_; }

private:
    Keyboard::Key key_ = Keyboard::Key::Undefined;
};

}  // namespace FA