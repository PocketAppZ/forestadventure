/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <memory>
#include <unordered_map>

#include "Draw/Animation.h"
#include "Draw/Image.h"
#include "Entity/Action.h"
#include "Entity/EventType.h"
#include "Enum/FaceDirection.h"
#include "Fwd/SfmlFwd.h"

namespace FA {

namespace Entity {

struct BasicEvent;
class Shape;
class EntityService;

class BasicMode
{
public:
    struct Direction
    {
        Animation animation_;
        Image image_;
    };

    BasicMode(EntityService& entityService);
    virtual ~BasicMode();

    BasicMode(const BasicMode&) = delete;
    BasicMode& operator=(const BasicMode&) = delete;
    BasicMode(BasicMode&&) = delete;
    BasicMode& operator=(BasicMode&&) = delete;

    virtual void Enter(std::shared_ptr<BasicEvent> event) {}
    virtual void Exit() {}
    virtual void Update(float deltaTime) {}
    virtual void DrawTo(sf::RenderTarget& renderTarget);
    virtual void Awake() {}

    virtual ModeType GetModeType() const = 0;

    void BindAction(const Action& action, EventType eventType);
    void BindActionDuringUpdate(const Action& action, std::function<bool(std::shared_ptr<Shape>)> condition);
    Action GetAction(EventType eventType) const;
    Action PollAction() const;
    Direction& AddDirection(FaceDirection faceDirection);

protected:
    EntityService& Service() const { return entityService_; }
    Animation GetAnimation(FaceDirection faceDirection) const;
    Image GetImage(FaceDirection faceDirection) const;

private:
    EntityService& entityService_;
    std::unordered_map<EventType, Action> eventMap_;
    std::function<bool(std::shared_ptr<Shape>)> actionCondition_ = [](std::shared_ptr<Shape>) { return false; };
    Action nextAction_{};
    std::unordered_map<FaceDirection, Direction> directions_;
};

}  // namespace Entity

}  // namespace FA
