#ifndef NEBULA_UDP_NETWORK_LAYER_H
#define NEBULA_UDP_NETWORK_LAYER_H

#include "nebula/NetworkLayer.h"
#include <thread>
#include <atomic>

namespace nebula {

class UdpNetworkLayer : public NetworkLayer {
private:
    int port_;
    int socketFd_;
    std::atomic<bool> running_;
    std::thread listenThread_;
    std::function<void(const std::string&)> receiveCallback_;

    void listenLoop();

public:
    UdpNetworkLayer(int port);
    ~UdpNetworkLayer() override;

    void start() override;
    void stop() override;
    void broadcast(const std::string& message) override;
    void setReceiveCallback(std::function<void(const std::string&)> callback) override;
};

} // namespace nebula

#endif // NEBULA_UDP_NETWORK_LAYER_H
