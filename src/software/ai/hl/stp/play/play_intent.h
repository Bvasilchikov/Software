#pragma once
#include "software/geom/point.h"
#include "software/world/world.h"

enum IntentType
{
    PASS = 0,
    SHOT = 1,
    DRIBBLE = 2
    // TODO: useless
};

class PlayIntent
{
public:

    explicit PlayIntent(IntentType play_intent_type, const World &world);

    const Point& getBallDestination() const;
    IntentType getIntentType() const;

private:
    const IntentType intent;
    const World& world; // TODO:  rename to initial_world
};


