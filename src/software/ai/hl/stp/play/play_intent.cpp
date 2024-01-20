#include "play_intent.h"


PlayIntent::PlayIntent(IntentType play_intent_type, const World &world)
    : intent(play_intent_type),
      world(world)
{
}

const Point& PlayIntent::getBallDestination() const
{
    return ball_destination;
}

IntentType PlayIntent::getIntentType() const
{
    return intent;
}
