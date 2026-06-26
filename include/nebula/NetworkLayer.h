#ifndef NEBULA_NETWORK_LAYER_H
#define NEBULA_NETWORK_LAYER_H

#include <string>
#include <functional>

namespace nebula {

/**
 * @brief Abstract interface simulating peer discovery, message broadcasting,
 *        and task gossip between nodes.
 */
class NetworkLayer {
public:
    virtual ~NetworkLayer() = default;

    /**
     * @brief Start listening for network messages.
     */
    virtual void start() = 0;

    /**
     * @brief Stop the network listener.
     */
    virtual void stop() = 0;

    /**
     * @brief Broadcast a message to other nodes on the network.
     * @param message The message to broadcast.
     */
    virtual void broadcast(const std::string& message) = 0;

    /**
     * @brief Register a callback to be invoked when a message is received.
     * @param callback The function to call with the received message.
     */
    virtual void setReceiveCallback(std::function<void(const std::string&)> callback) = 0;
};

} // namespace nebula

#endif // NEBULA_NETWORK_LAYER_H
