/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "BasicParseHelper.h"

namespace FA {

namespace Tile {

template <class ElementT, class ErrorT>
class ParseHelper : BasicParseHelper<ElementT, ErrorT>
{
public:
    virtual bool ParseTileSet(ElementT* element, ParsedTileSetData& data) const override
    {
        const char* name = nullptr;
        auto r0 = element->QueryStringAttribute("name", &name);
        if (name) data.name_ = name;
        auto r1 = element->QueryAttribute("tilewidth", &data.tileWidth_);
        auto r2 = element->QueryAttribute("tileheight", &data.tileHeight_);
        auto r3 = element->QueryAttribute("tilecount", &data.tileCount_);
        auto r4 = element->QueryAttribute("columns", &data.columns_);

        std::vector<ErrorT> results{r0, r1, r2, r3, r4};
        return Result(results);
    }

    virtual bool ParseImage(ElementT* element, ParsedImage& image) const override
    {
        const char* source = nullptr;
        auto r0 = element->QueryStringAttribute("source", &source);
        if (source) image.source_ = source;
        auto r1 = element->QueryAttribute("width", &image.width_);
        auto r2 = element->QueryAttribute("height", &image.height_);

        std::vector<ErrorT> results{r0, r1, r2};
        return Result(results);
    }

    virtual bool ParseTile(ElementT* element, ParsedTile& tile) const override
    {
        auto r0 = element->QueryAttribute("id", &tile.id_);
        ElementT* imageElement = element->FirstChildElement("image");
        auto i = ParseImage(imageElement, tile.image_);

        return ((r0 == ErrorT::XML_SUCCESS) && i);
    }

    virtual bool ParseTiles(ElementT* element, std::vector<ParsedTile>& tiles) const override
    {
        ElementT* e = element;
        bool result = true;

        while (e != nullptr) {
            ParsedTile tile;
            auto r = ParseTile(e, tile);
            result &= r;
            tiles.push_back(tile);
            e = e->NextSiblingElement("tile");
        }

        return result;
    }

private:
    bool Result(const std::vector<ErrorT>& results) const
    {
        return std::all_of(results.begin(), results.end(), [](const ErrorT& e) { return e == ErrorT::XML_SUCCESS; });
    }
};

}  // namespace Tile

}  // namespace FA
