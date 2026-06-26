#include "UdpNetworkLayer.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>

namespace nebula {

UdpNetworkLayer::UdpNetworkLayer(int port) : port_(port), socketFd_(-1), running_(false) {}

UdpNetworkLayer::~UdpNetworkLayer() {
    stop();
}

void UdpNetworkLayer::setReceiveCallback(std::function<void(const std::string&)> callback) {
    receiveCallback_ = callback;
}

void UdpNetworkLayer::start() {
    if (running_) return;

    socketFd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd_ < 0) {
        std::cerr << "[UdpNetworkLayer] Failed to create socket." << std::endl;
        return;
    }

    // Allow multiple sockets to use the same PORT number (useful for local testing)
    int opt = 1;
    if (setsockopt(socketFd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        std::cerr << "[UdpNetworkLayer] Failed to set SO_REUSEPORT." << std::endl;
    }

    // Enable broadcasting
    int broadcastEnable = 1;
    if (setsockopt(socketFd_, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        std::cerr << "[UdpNetworkLayer] Failed to enable broadcast." << std::endl;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port_);

    if (bind(socketFd_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "[UdpNetworkLayer] Failed to bind socket to port " << port_ << std::endl;
        close(socketFd_);
        return;
    }

    running_ = true;
    listenThread_ = std::thread(&UdpNetworkLayer::listenLoop, this);
    std::cout << "[UdpNetworkLayer] Started listening on port " << port_ << std::endl;
}

void UdpNetworkLayer::stop() {
    if (!running_) return;

    running_ = false;

    if (socketFd_ >= 0) {
        // Close socket to unblock recvfrom
        shutdown(socketFd_, SHUT_RDWR);
        close(socketFd_);
        socketFd_ = -1;
    }

    if (listenThread_.joinable()) {
        listenThread_.join();
    }

    std::cout << "[UdpNetworkLayer] Stopped networking on port " << port_ << std::endl;
}

void UdpNetworkLayer::broadcast(const std::string& message) {
    if (socketFd_ < 0) {
        std::cerr << "[UdpNetworkLayer] Socket not initialized. Call start() first." << std::endl;
        return;
    }

    sockaddr_in broadcastAddr{};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(port_);
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");

    if (sendto(socketFd_, message.c_str(), message.size(), 0, (struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
        std::cerr << "[UdpNetworkLayer] Failed to broadcast message." << std::endl;
    } else {
        std::cout << "[UdpNetworkLayer] Broadcasted message: " << message << std::endl;
    }
}

void UdpNetworkLayer::listenLoop() {
    char buffer[1024];
    while (running_) {
        sockaddr_in senderAddr{};
        socklen_t senderLen = sizeof(senderAddr);

        ssize_t bytesRead = recvfrom(socketFd_, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&senderAddr, &senderLen);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::string msg(buffer);
            if (receiveCallback_) {
                receiveCallback_(msg);
            }
        } else if (bytesRead < 0 && running_) {
            // Error occurred
            break;
        }
    }
}

} // namespace nebula
