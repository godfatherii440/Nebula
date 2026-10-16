#ifndef NEBULA_TCP_NETWORK_ENGINE_H
#define NEBULA_TCP_NETWORK_ENGINE_H

#include "nebula/NetworkEngine.h"
#include <atomic>
#include <thread>
#include <functional>
#include <string>

namespace nebula {

class TcpServer : public NetworkServer {
private:
    int serverFd_;
    int port_;
    std::atomic<bool> running_;
    std::thread acceptThread_;
    std::function<std::string(const std::string&)> messageHandler_;

    void listenLoop();

public:
    TcpServer();
    ~TcpServer() override;

    bool start(int port) override;
    void stop() override;
    void setMessageHandler(std::function<std::string(const std::string&)> handler) override;
};

class TcpClient : public NetworkClient {
private:
    int clientFd_;
    bool connected_;

public:
    TcpClient();
    ~TcpClient() override;

    bool connectTo(const std::string& host, int port) override;
    std::string sendAndReceive(const std::string& message) override;
    void disconnect() override;
};

} // namespace nebula

#endif // NEBULA_TCP_NETWORK_ENGINE_H
