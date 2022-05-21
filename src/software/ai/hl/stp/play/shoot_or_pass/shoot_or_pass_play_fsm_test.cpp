#include "software/ai/hl/stp/play/shoot_or_pass/shoot_or_pass_play_fsm.h"

#include <gtest/gtest.h>

#include "shared/parameter/cpp_dynamic_parameters.h"
#include "software/test_util/equal_within_tolerance.h"
#include "software/test_util/test_util.h"

TEST(ShootOrPassPlayFSMTest, test_transitions)
{
    World world = ::TestUtil::createBlankTestingWorld();

    FSM<ShootOrPassPlayFSM> fsm(
        ShootOrPassPlayFSM{std::make_shared<const ThunderbotsConfig>()->getAiConfig()});
    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::StartState>));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 3, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::AttemptShotState>));
}

TEST(ShootOrPassPlayFSMTest, test_abort_pass_guard_lost_posession)
{
    World world = ::TestUtil::createBlankTestingWorld();
    world.updateRefereeCommand(RefereeCommand::FORCE_START);

    FSM<ShootOrPassPlayFSM> fsm(
        ShootOrPassPlayFSM{std::make_shared<const ThunderbotsConfig>()->getAiConfig()});
    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::StartState>));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 4, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));
    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::AttemptShotState>));

    world.updateBall(Ball(Point(-1, 0), Vector(0, 0), Timestamp::fromSeconds(1)));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 2, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    world.setTeamWithPossession(TeamSide::FRIENDLY);
    Robot friendly_robot_1(1, Point(3, 0), Vector(0, 0), Angle::zero(),
                           AngularVelocity::zero(), Timestamp::fromSeconds(2));
    Robot friendly_robot_2(2, Point(0, 0), Vector(0, 0), Angle::half(),
                           AngularVelocity::zero(), Timestamp::fromSeconds(2));
    std::vector<Robot> friendlies = {friendly_robot_1, friendly_robot_2};
    world.updateFriendlyTeamState(Team(friendlies));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 2, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 3, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::TakePassState>));

    world.setTeamWithPossession(TeamSide::ENEMY);

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 2, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::AttemptShotState>));
}

TEST(ShootOrPassPlayFSMTest, test_abort_pass_guard_ball_changed)
{
    World world = ::TestUtil::createBlankTestingWorld();
    world.updateRefereeCommand(RefereeCommand::FORCE_START);

    FSM<ShootOrPassPlayFSM> fsm(
        ShootOrPassPlayFSM{std::make_shared<const ThunderbotsConfig>()->getAiConfig()});
    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::StartState>));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 4, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));
    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::AttemptShotState>));

    world.updateBall(Ball(Point(-1, 0), Vector(0, 0), Timestamp::fromSeconds(1)));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 2, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    world.setTeamWithPossession(TeamSide::FRIENDLY);
    Robot friendly_robot_1(1, Point(3, 0), Vector(0, 0), Angle::zero(),
                           AngularVelocity::zero(), Timestamp::fromSeconds(2));
    Robot friendly_robot_2(2, Point(0, 0), Vector(0, 0), Angle::half(),
                           AngularVelocity::zero(), Timestamp::fromSeconds(2));
    std::vector<Robot> friendlies = {friendly_robot_1, friendly_robot_2};
    world.updateFriendlyTeamState(Team(friendlies));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 2, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 3, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::TakePassState>));

    world.updateBall(Ball(Point(1, 0), Vector(0, 0), Timestamp::fromSeconds(1)));


    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 2, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::AttemptShotState>));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 2, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));


    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::AttemptShotState>));
}


TEST(ShootOrPassPlayFSMTest, test_took_shot_guard)
{
    World world = ::TestUtil::createBlankTestingWorld();

    FSM<ShootOrPassPlayFSM> fsm(
        ShootOrPassPlayFSM{std::make_shared<const ThunderbotsConfig>()->getAiConfig()});
    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::StartState>));

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 3, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    EXPECT_TRUE(fsm.is(boost::sml::state<ShootOrPassPlayFSM::AttemptShotState>));


    Robot friendly_robot_1(1, Point(5, 0), Vector(0, 0), Angle::zero(),
                           AngularVelocity::zero(), Timestamp());
    std::vector<Robot> friendlies = {friendly_robot_1};
    world.updateFriendlyTeamState(Team(friendlies));
    world.updateBall(Ball(Point(5.0, 0), Vector(10, 0), Timestamp::fromSeconds(1)));
    world.setTeamWithPossession(TeamSide::FRIENDLY);

    fsm.process_event(ShootOrPassPlayFSM::Update(
        ShootOrPassPlayFSM::ControlParams{},
        PlayUpdate(
            world, 3, [](PriorityTacticVector new_tactics) {}, InterPlayCommunication{},
            [](InterPlayCommunication comm) {})));

    // friendly robot is in front of goal, no other robots to pass to,
    // he takes the shot and triggers the tookShot guard, fsm goes into termination state
    EXPECT_TRUE(fsm.is(boost::sml::state<boost::sml::back::terminate_state>));
}
