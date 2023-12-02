
#include "software/ai/hl/stp/play/play_monitor.h"
#include "software/geom/point.h"

PlayMoitor::PlayMoitor()
{
}

double PlayMoitor::calculateIntentBallScore()
{
    Point startPos             = intent.startWorld.getBallPosition();
    double distBetweenStartEnd = distanceBetween(world.ball_position, intent.position);
    double distToDest          = distanceBetween(world.ball_position, intent.position);
    return distToDest / distBetweenStartEnd;
}

double PlayMoitor::calculateIntentActionScore()
{
    double ballScore = calculateIntentBallScore();
    switch (intent.type)
    {
        case PASS:
            return ballScore;
        case SHOT:
            return 1.0;
            ifscored, ballScore;
            otherwise;
        case DRIBBLE:
            return ballScore * robotMovedToPosition(intent.postion);
    }
}
