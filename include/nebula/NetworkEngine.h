#ifndef NEBULA_NETWORK_ENGINE_H
#define NEBULA_NETWORK_ENGINE_H

#include <string>
#include <functional>

namespace nebula {

/**
 * @brief Interface for TCP/P2P socket communication server.
 */
class NetworkServer {
public:
    virtual ~NetworkServer() = default;

    /**
     * @brief Start listening on specified TCP port.
     */
    virtual bool start(int port) = 0;

    /**
     * @brief Stop socket server.
     */
    virtual void stop() = 0;

    /**
     * @brief Register message handler callback.
     */
    virtual void setMessageHandler(std::function<std::string(const std::string&)> handler) = 0;
};

/**
 * @brief Interface for TCP/P2P socket communication client.
 */
class NetworkClient {
public:
    virtual ~NetworkClient() = default;

    /**
     * @brief Connect to remote host and port.
     */
    virtual bool connectTo(const std::string& host, int port) = 0;

    /**
     * @brief Send message over socket and receive response.
     */
    virtual std::string sendAndReceive(const std::string& message) = 0;

    /**
     * @brief Disconnect from server.
     */
    virtual void disconnect() = 0;
};

} // namespace nebula

#endif // NEBULA_NETWORK_ENGINE_H
