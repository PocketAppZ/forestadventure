/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <string>

#include <SFML/Graphics/Rect.hpp>

#include "Fwd/SfmlFwd.h"
#include "TileMapData.h"

namespace FA {

class TextureManager;

namespace Tile {

class TmxParser;

class TileMapReader
{
public:
    TileMapReader(const std::string& fileName);
    ~TileMapReader();

    void Load(TextureManager& textureManager);
    TileMapData GetTileMapData() const { return tileMapData_; }

private:
    std::string fileName_{};
    TileMapData tileMapData_;

private:
    void ReadMapProperties(const TmxParser& tmxParser);
    void ReadTileSets(const TmxParser& tmxParser, TextureManager& textureManager);
    void ReadLayers(const TmxParser& tmxParser);
    std::string GetFilePath(const std::string& baseDir, const std::string& source) const;
};

}  // namespace Tile

}  // namespace FA
