#include "agent.h"

Agent::Agent(RobotId robot_id, RobotState robot_state, TeamSide side,
             RobotPath &path, double radius, double max_speed,
             double max_accel, double max_radius_inflation) :
        robot_id(robot_id),
        robot_state(robot_state),
        side(side),
        path(path),
        radius(radius),
        min_radius(radius),
        max_speed(max_speed),
        max_accel(max_accel),
        max_radius_inflation(max_radius_inflation),
        new_velocity() {

}

void Agent::update(double time_step) {
//    LOG(INFO) << "new velocity";
    LOG(WARNING) << "before normalization " << new_velocity;
//    new_velocity = Vector(1,1);
//    return;
    if (new_velocity.length() >= max_speed) {
        // New velocity can not be greater than max speed
        new_velocity = new_velocity.normalize(max_speed);
    }

    const Vector dv = new_velocity - robot_state.velocity();
    if (dv.length() < (max_accel * time_step) || dv.length() == 0.0) {
        LOG(WARNING) << "updating old vel from " << robot_state.velocity() << " to " << new_velocity;
//        robot_state.velocity().set(new_velocity.x(), new_velocity.y());
        robot_state = RobotState(robot_state.position(), new_velocity, robot_state.orientation(), robot_state.angularVelocity());
        LOG(WARNING) << "updated " << robot_state.velocity();


    } else {
        // Calculate the maximum velocity towards the preferred velocity, given the
        // acceleration constraint
        LOG(WARNING) << "wow" << robot_state.velocity() << " to " <<  robot_state.velocity() + (max_accel * time_step) * (dv / dv.length());
//        robot_state.velocity() = robot_state.velocity() + ((max_accel * time_step) * (dv / dv.length()));
        robot_state = RobotState(robot_state.position(),  robot_state.velocity() + (max_accel * time_step) * (dv / dv.length()), robot_state.orientation(), robot_state.angularVelocity());

    }
    robot_state.position() = robot_state.position() + (robot_state.velocity() * time_step);

    Point current_dest;

    const std::optional<PathPoint> &path_point = path.getCurrentPathPoint();
    if (path_point == std::nullopt) {
        // If there are no destinations, the robot should stay at its current position
        current_dest = robot_state.position();
    } else {
        current_dest = path_point.value().getPosition();
    }

    if ((current_dest - robot_state.position()).lengthSquared() <
        path.getPathRadius() * path.getPathRadius() && !path.isGoingToFinalPathPoint()) {
        // Is at current goal position
        path.incrementPathIndex();

    }
}

void Agent::updateRadiusFromVelocity() {
    // Linearly increase radius based on the current agent velocity
    radius = min_radius + max_radius_inflation * (robot_state.velocity().length() / max_speed);
}

double Agent::getMaxAccel() const {
    return max_accel;
}

double Agent::getMaxSpeed() const {
    return max_speed;
}

double Agent::getRadius() const {
    return radius;
}

RobotState Agent::getRobotState() const {
    return robot_state;
}

RobotPath &Agent::getPath() {
    return path;
}

Vector Agent::getPreferredVelocity() const {
    return preferred_velocity;
}

void Agent::setPreferredVelocity(Vector velocity) {
    preferred_velocity = velocity;
}


