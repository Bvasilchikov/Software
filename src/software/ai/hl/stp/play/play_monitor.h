#pragma once
#include "play_intent.h"
#include "software/world/world.h"

/**
 * Represents a class that monitors and computes a sucess score for a running play
 */
class PlayMonitor
{
public:
    explicit PlayMonitor(const PlayIntent& initialIntent);

    void startMonitoring(const World& initialWorld, PlayIntent initialIntent);
    double endMonitoring();
    void updateWorld(const World& newWorld);
    void updatePlayIntent(const PlayIntent& newIntent);

private:
    double calculateIntentBallScore();
    double calculateIntentActionScore();

    PlayIntent intent;
    World world;
};
