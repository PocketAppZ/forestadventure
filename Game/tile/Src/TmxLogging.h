/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "Format.h"
#include "Logger.h"

#include <sstream>

namespace FA {

namespace Tile {

LogLib::BasicLogger& TmxLog();

}  // namespace Tile

}  // namespace FA

#define LOG_TMXINFO(...) FA::Tile::TmxLog().MakeInfoLogEntry(__FUNCTION__, LogLib::Format::ToString(__VA_ARGS__))
#define LOG_TMXWARN(...) FA::Tile::TmxLog().MakeWarnLogEntry(__FUNCTION__, LogLib::Format::ToString(__VA_ARGS__))
#define LOG_TMXERROR(...) FA::Tile::TmxLog().MakeErrorLogEntry(__FUNCTION__, LogLib::Format::ToString(__VA_ARGS__))

#define LOG_TMXVARIABLE(variable)                       \
    std::ostringstream oss;                             \
    oss << "{" << #variable << ": " << variable << "}"; \
    FA::Tile::TmxLog().MakeDebugLogEntry(__FUNCTION__, oss.str())
