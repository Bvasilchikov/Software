#include "software/ai/hl/stp/play/play_monitor.h"
#include "software/geom/algorithms/distance.h"

PlayMonitor::PlayMonitor(const PlayIntent& initialIntent)
    : intent(initialIntent), world()
{
}

void PlayMonitor::startMonitoring(const World& initialWorld,
                                  PlayIntent initialIntent)
{
    world  = initialWorld;
    intent = initialIntent;
}

double PlayMonitor::endMonitoring()
{
    return 0.0;
}

void PlayMonitor::updateWorld(const World& newWorld)
{
    world = newWorld;
}

void PlayMonitor::updatePlayIntent(const PlayIntent& newIntent)
{
    intent = newIntent;
}

double PlayMonitor::calculateIntentBallScore()
{
    const double distBetweenStartEnd = distance(world.ball().position(),
                                                intent.getBallDestination());
    const double distToDest = distance(world.ball().position(),
                                       intent.getBallDestination());
    return distToDest / distBetweenStartEnd;
}

double PlayMonitor::calculateIntentActionScore()
{
    double ballScore = this->calculateIntentBallScore();
    switch (intent)
    {
        case PASS:
        case SHOT:
            return ballScore;
            return ballScore;
        case DRIBBLE:
            return ballScore * robotAtPosition(intent.postion);
            // TODO add dribbler id to intent

        default:
            return 0.0;
    }
}
