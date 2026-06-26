#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>

#include "nebula/Task.h"
#include "nebula/ExecutionEngine.h"
#include "nebula/NodeAgent.h"
#include "nebula/Scheduler.h"

// We need the mock headers for instantiation
#include "workload/DummyTask.h"
#include "engine/MockExecutionEngine.h"
#include "node_agent/MockNodeAgent.h"
#include "scheduler/BasicScheduler.h"
#include "verifier/MockVerifier.h"
#include "payment/MockPaymentManager.h"
#include "network/UdpNetworkLayer.h"

using namespace nebula;

int main(int argc, char* argv[]) {
    bool runAsNetworkNode = (argc > 1 && std::string(argv[1]) == "--node");

    std::cout << "==========================================" << std::endl;
    std::cout << " Nebula Compute - MVP Prototype v0.1      " << std::endl;
    std::cout << "==========================================" << std::endl;

    if (runAsNetworkNode) {
        std::cout << "--- Starting Network Node ---" << std::endl;
        UdpNetworkLayer network(8080);

        network.setReceiveCallback([](const std::string& msg) {
            std::cout << "[NetworkNode] Received P2P Message: " << msg << std::endl;
        });

        network.start();

        // Broadcast presence
        network.broadcast("NODE_DISCOVERY: new_node_online");

        // Wait for 2 seconds to receive potential messages
        std::this_thread::sleep_for(std::chrono::seconds(2));
        network.stop();

    } else {
        // 1. Create the scheduler
        auto scheduler = std::make_shared<BasicScheduler>();

        // Setup Verifier and Payment Manager
        auto verifier = std::make_shared<MockVerifier>();
        auto paymentManager = std::make_shared<MockPaymentManager>();
        scheduler->setVerifier(verifier);
        scheduler->setPaymentManager(paymentManager);

        // 2. Setup the execution engine
        auto engine = std::make_shared<MockExecutionEngine>();

        // 3. Create node agents and register them with the scheduler
        auto node1 = std::make_shared<MockNodeAgent>("node-alpha", engine, false); // CPU node
        auto node2 = std::make_shared<MockNodeAgent>("node-beta", engine, true);   // GPU node

        scheduler->registerNode(node1);
        scheduler->registerNode(node2);

        // 4. Create some tasks and submit them
        std::vector<std::shared_ptr<Task>> tasks;
        tasks.push_back(std::make_shared<DummyTask>("task-001", "CPU_WORKLOAD_A", false));
        tasks.push_back(std::make_shared<DummyTask>("task-002", "GPU_WORKLOAD_B", true));
        tasks.push_back(std::make_shared<DummyTask>("task-003", "CPU_WORKLOAD_C", false));

        for (const auto& task : tasks) {
            scheduler->submitTask(task);
        }

        // 5. Run the scheduler to assign and execute tasks
        std::cout << "\n--- Starting Execution ---" << std::endl;
        scheduler->scheduleTasks();
        std::cout << "--- Execution Complete ---\n" << std::endl;

        // 6. Verify results
        std::cout << "--- Results ---" << std::endl;
        for (const auto& task : tasks) {
            std::cout << "Task " << task->getId() << " -> ";
            if (task->isCompleted()) {
                std::cout << "Completed: " << task->getResult() << " | Verified: " << (task->isVerified() ? "Yes" : "No") << std::endl;
            } else {
                std::cout << "Pending" << std::endl;
            }
        }
    }

    return 0;
}
