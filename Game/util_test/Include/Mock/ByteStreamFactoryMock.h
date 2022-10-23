/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <gmock/gmock.h>

#include "BasicByteStreamFactory.h"

namespace FA {

class ByteStreamFactoryMock : public BasicByteStreamFactory
{
public:
    MOCK_METHOD(std::unique_ptr<BasicByteStream>, Create, (const std::string& fileName), (const override));
};

class ByteStreamFactoryMockProxy : public BasicByteStreamFactory
{
public:
    ByteStreamFactoryMockProxy(ByteStreamFactoryMock& mock)
        : mock_(mock)
    {}

    std::unique_ptr<BasicByteStream> Create(const std::string& fileName) const override
    {
        return mock_.Create(fileName);
    }

private:
    ByteStreamFactoryMock& mock_;
};

}  // namespace FA