/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "TmxLogging.h"
#include "Util/Folder.h"

namespace FA {

LogLib::Logger& TmxLog()
{
    static LogLib::Logger tmxLog;

    static bool once = []() {
        tmxLog.OpenLog(FA::GetLogPath(), "tmx-log.txt", false);
        return true;
    }();

    return tmxLog;
}

}  // namespace FA
