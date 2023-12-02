#pragma once


class PlayMonitor
{
    public:
    explicit PlayMonitor();
    void startMonitoring(world World, intent PlayIntent);
    double endMonitoring();

    void updateWorld(const World& world);
    void updatePlayIntent(const PlayIntent& PlayIntent);

    private:
    double calculateIntentBallScore();
    double calculateIntentActionScore();
};
