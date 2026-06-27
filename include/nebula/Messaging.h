#ifndef NEBULA_MESSAGING_H
#define NEBULA_MESSAGING_H

#include <string>
#include <memory>
#include <functional>
#include "nebula/Task.h"

namespace nebula {

/**
 * @brief Represents the messaging and queuing layer (e.g., Kafka, Redis Streams)
 *        for resilient task distribution and buffering.
 */
class Messaging {
public:
    virtual ~Messaging() = default;

    /**
     * @brief Connect to the messaging broker.
     */
    virtual void connect(const std::string& brokerAddress) = 0;

    /**
     * @brief Publish a task to a specific topic/stream.
     * @param topic The topic to publish to.
     * @param task The task to publish.
     */
    virtual void publishTask(const std::string& topic, std::shared_ptr<Task> task) = 0;

    /**
     * @brief Subscribe to a topic to receive tasks.
     * @param topic The topic to subscribe to.
     * @param callback The function to call when a task is received.
     */
    virtual void subscribe(const std::string& topic, std::function<void(std::shared_ptr<Task>)> callback) = 0;

    /**
     * @brief Disconnect from the messaging broker.
     */
    virtual void disconnect() = 0;
};

} // namespace nebula

#endif // NEBULA_MESSAGING_H
