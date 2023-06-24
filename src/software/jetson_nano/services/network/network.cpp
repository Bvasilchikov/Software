#include "software/jetson_nano/services/network/network.h"

NetworkService::NetworkService(const std::string& ip_address,
                               unsigned short world_listener_port,
                               unsigned short primitive_listener_port,
                               unsigned short robot_status_sender_port, bool multicast)
    : primitive_tracker(ProtoTracker("primitive set")),
      world_tracker(ProtoTracker("world"))
{
    sender = std::make_unique<ThreadedProtoUdpSender<TbotsProto::RobotStatus>>(
        ip_address, robot_status_sender_port, multicast);
    listener_primitive_set =
        std::make_unique<ThreadedProtoUdpListener<TbotsProto::PrimitiveSet>>(
            ip_address, primitive_listener_port,
            boost::bind(&NetworkService::primitiveSetCallback, this, _1), multicast);
    listener_world = std::make_unique<ThreadedProtoUdpListener<TbotsProto::World>>(
        ip_address, world_listener_port,
        boost::bind(&NetworkService::worldCallback, this, _1), multicast);
}

std::tuple<TbotsProto::PrimitiveSet, TbotsProto::World> NetworkService::poll(
    const TbotsProto::RobotStatus& robot_status)
{
    std::scoped_lock lock{primitive_set_mutex, world_mutex};
    // Rate limit sending of proto based on thunderloop freq
    if ((robot_status.motor_status().front_left().motor_faults_size() > 0 ||
         robot_status.motor_status().front_right().motor_faults_size() > 0 ||
         robot_status.motor_status().back_left().motor_faults_size() > 0 ||
         robot_status.motor_status().back_right().motor_faults_size() > 0) ||
        (network_ticks / (thunderloop_ticks + 1.0) <=
         ROBOT_STATUS_TO_THUNDERLOOP_HZ_RATIO))
    {
        sender->sendProto(robot_status);
        network_ticks = (network_ticks + 1) % ROBOT_STATUS_BROADCAST_RATE_HZ;
    }
    thunderloop_ticks = (thunderloop_ticks + 1) % CONTROL_LOOP_HZ;
    return std::tuple<TbotsProto::PrimitiveSet, TbotsProto::World>{primitive_set_msg,
                                                                   world_msg};
}

void NetworkService::primitiveSetCallback(TbotsProto::PrimitiveSet input)
{
    std::scoped_lock<std::mutex> lock(primitive_set_mutex);
    const uint64_t seq_num = input.sequence_number();

    primitive_tracker.send(seq_num);
    if (primitive_tracker.isLastValid())
    {
        primitive_set_msg = input;
    }

    float primitive_set_loss_rate = primitive_tracker.getLossRate();
    if (primitive_set_loss_rate > PROTO_LOSS_WARNING_THRESHOLD)
    {
        LOG(WARNING) << "Primitive set loss rate is " << primitive_set_loss_rate * 100
                     << "%";
    }
}

void NetworkService::worldCallback(TbotsProto::World input)
{
    std::scoped_lock<std::mutex> lock(world_mutex);
    const uint64_t seq_num = input.sequence_number();

    world_tracker.send(seq_num);
    if (world_tracker.isLastValid())
    {
        world_msg = input;
    }

    float world_loss_rate = world_tracker.getLossRate();
    if (world_loss_rate > PROTO_LOSS_WARNING_THRESHOLD)
    {
        LOG(WARNING) << "World loss rate is " << world_loss_rate * 100 << "%";
    }
}
