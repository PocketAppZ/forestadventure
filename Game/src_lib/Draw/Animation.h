/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <vector>

#include <SFML/Graphics/Rect.hpp>

#include "Fwd/SfmlFwd.h"

namespace FA {

class Animation
{
public:
    Animation() = default;
    Animation(const sf::Texture* texture, const std::vector<sf::IntRect>& frames, unsigned int defaultFrame,
              float switchTime);

    void Update(float deltaTime);  // delta time; time since previous time to current frame
    void ApplyTo(sf::Sprite& sprite);
    void Start();
    void Stop();
    bool IsCompleted() const;
    bool IsValid() const;

private:
    const sf::Texture* texture_ = nullptr;
    bool isStopped_ = true;
    float time_{};        // time since we last switched frame
    float switchTime_{};  // time before to switch to next frame
    unsigned int iFrame_{};
    std::vector<sf::IntRect> frames_;
    unsigned int nFrames_{};
    unsigned int defaultFrame_{};
    bool isCompleted_ = false;
    bool isValid_ = false;
};

}  // namespace FA