#include "TcpNetworkEngine.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <sys/select.h>

namespace nebula {

TcpServer::TcpServer() : serverFd_(-1), port_(0), running_(false) {}

TcpServer::~TcpServer() {
    stop();
}

bool TcpServer::start(int port) {
    port_ = port;
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd_ < 0) {
        std::cerr << "[TcpServer] Failed to create socket." << std::endl;
        return false;
    }

    int opt = 1;
    if (setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "[TcpServer] setsockopt failed." << std::endl;
        close(serverFd_);
        serverFd_ = -1;
        return false;
    }

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(serverFd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "[TcpServer] Bind failed on port " << port_ << std::endl;
        close(serverFd_);
        serverFd_ = -1;
        return false;
    }

    if (listen(serverFd_, 5) < 0) {
        std::cerr << "[TcpServer] Listen failed." << std::endl;
        close(serverFd_);
        serverFd_ = -1;
        return false;
    }

    running_ = true;
    acceptThread_ = std::thread(&TcpServer::listenLoop, this);
    std::cout << "[TcpServer] Started listening on port " << port_ << std::endl;
    return true;
}

void TcpServer::listenLoop() {
    while (running_) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverFd_, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // 100ms timeout

        int activity = select(serverFd_ + 1, &readfds, NULL, NULL, &timeout);
        if (activity > 0 && FD_ISSET(serverFd_, &readfds)) {
            sockaddr_in clientAddr;
            socklen_t addrLen = sizeof(clientAddr);
            int clientSocket = accept(serverFd_, (struct sockaddr*)&clientAddr, &addrLen);

            if (clientSocket >= 0) {
                char buffer[2048] = {0};
                ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead > 0) {
                    std::string request(buffer, bytesRead);
                    std::string response = "ACK";
                    if (messageHandler_) {
                        response = messageHandler_(request);
                    }
                    send(clientSocket, response.c_str(), response.length(), 0);
                }
                close(clientSocket);
            }
        }
    }
}

void TcpServer::stop() {
    if (running_) {
        running_ = false;
        if (acceptThread_.joinable()) {
            acceptThread_.join();
        }
    }
    if (serverFd_ >= 0) {
        close(serverFd_);
        serverFd_ = -1;
    }
    std::cout << "[TcpServer] Server stopped." << std::endl;
}

void TcpServer::setMessageHandler(std::function<std::string(const std::string&)> handler) {
    messageHandler_ = handler;
}

// TcpClient Implementation

TcpClient::TcpClient() : clientFd_(-1), connected_(false) {}

TcpClient::~TcpClient() {
    disconnect();
}

bool TcpClient::connectTo(const std::string& host, int port) {
    clientFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFd_ < 0) {
        std::cerr << "[TcpClient] Failed to create socket." << std::endl;
        return false;
    }

    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &servAddr.sin_addr) <= 0) {
        if (host == "localhost") {
            inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
        } else {
            std::cerr << "[TcpClient] Invalid address or address not supported: " << host << std::endl;
            close(clientFd_);
            clientFd_ = -1;
            return false;
        }
    }

    if (connect(clientFd_, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        std::cerr << "[TcpClient] Connection failed to " << host << ":" << port << std::endl;
        close(clientFd_);
        clientFd_ = -1;
        return false;
    }

    connected_ = true;
    return true;
}

std::string TcpClient::sendAndReceive(const std::string& message) {
    if (!connected_ || clientFd_ < 0) {
        return "";
    }

    if (send(clientFd_, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "[TcpClient] Send failed." << std::endl;
        return "";
    }

    char buffer[2048] = {0};
    ssize_t bytesRead = recv(clientFd_, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        return std::string(buffer, bytesRead);
    }

    return "";
}

void TcpClient::disconnect() {
    if (clientFd_ >= 0) {
        close(clientFd_);
        clientFd_ = -1;
    }
    connected_ = false;
}

} // namespace nebula
