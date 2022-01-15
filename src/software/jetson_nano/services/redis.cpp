#include "redis.h"

#include "software/logger/logger.h"

RedisService::RedisService(std::string host, size_t port)
    : subscriber(), host_(host), port_(port)
{
}

void RedisService::start()
{
    subscriber.connect(
        host_, port_,
        [](const std::string &host, std::size_t port, cpp_redis::connect_state status) {
            if (status == cpp_redis::connect_state::dropped)
            {
                LOG(WARNING) << "Connection dropped";
            }
            else if (status == cpp_redis::connect_state::failed)
            {
                LOG(WARNING) << "Connection failed";
            }
            else if (status == cpp_redis::connect_state::ok)
            {
                LOG(INFO) << "Connection successful";
            }
        });
}

void RedisService::stop()
{
    subscriber.disconnect(false);
}

void RedisService::subscribe(const std::string &channel,
                             void (*subscribe_callback)(std::string, std::string))
{
    subscriber.subscribe(channel, subscribe_callback);
}
