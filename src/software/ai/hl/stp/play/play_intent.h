#pragma once
#include "software/geom/point.h"

enum IntentType
{
    PASS = 0,
    SHOT = 1,
    DRIBBLE = 2
};

class PlayIntent
{
public:

    explicit PlayIntent(IntentType play_intent_type, const Point& ball_destination);

    const Point& getBallDestination() const;
    IntentType getIntentType() const;

private:
    const IntentType intent;
    const Point& ball_destination;
};


