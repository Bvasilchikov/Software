#include "play_intent.h"


PlayIntent::PlayIntent(IntentType play_intent_type, const Point& ball_destination)
    : intent(play_intent_type),
      ball_destination(ball_destination)
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
