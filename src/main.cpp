#include <iostream>
#include <memory>
#include <vector>

#include "nebula/Task.h"
#include "nebula/ExecutionEngine.h"
#include "nebula/NodeAgent.h"
#include "nebula/Scheduler.h"
#include "nebula/VerificationEngine.h"
#include "nebula/BillingEngine.h"
#include "nebula/NetworkEngine.h"

#include "workload/DummyTask.h"
#include "engine/MockExecutionEngine.h"
#include "node_agent/MockNodeAgent.h"
#include "scheduler/BasicScheduler.h"
#include "verification/ConsensusVerificationEngine.h"
#include "billing/UsageBillingEngine.h"
#include "network/TcpNetworkEngine.h"

using namespace nebula;

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << " Nebula Compute - Decentralized Supercomputer" << std::endl;
    std::cout << "==========================================" << std::endl;

    // 1. Setup Scheduler, Verification Engine, and Billing Engine
    auto scheduler = std::make_shared<BasicScheduler>();
    auto engine = std::make_shared<MockExecutionEngine>();
    auto verifier = std::make_shared<ConsensusVerificationEngine>();
    auto billing = std::make_shared<UsageBillingEngine>(0.70); // 70% provider payout share

    // Deposit client funds
    std::string clientId = "client-alpha";
    billing->depositFunds(clientId, 100.00);

    // 2. Setup Nodes with heterogeneous capacities
    ResourceSpec cpuCap(4, 0, 8192, "CPU");
    ResourceSpec gpuCap(8, 2, 16384, "GPU_STANDARD");

    auto node1 = std::make_shared<MockNodeAgent>("node-alpha", engine, cpuCap, 0.95);
    auto node2 = std::make_shared<MockNodeAgent>("node-beta", engine, gpuCap, 1.00);

    scheduler->registerNode(node1);
    scheduler->registerNode(node2);

    // 3. Create micro-tasks (CPU and GPU)
    ResourceSpec cpuReq(2, 0, 2048, "CPU");
    ResourceSpec gpuReq(4, 1, 4096, "GPU_STANDARD");

    auto task1 = std::make_shared<DummyTask>("task-cpu-01", "CPU_MATRIX_MULT", cpuReq, 1.5);
    auto task2 = std::make_shared<DummyTask>("task-gpu-01", "GPU_INFERENCE", gpuReq, 3.0);

    // Charge client for submitted tasks
    double cost1 = billing->calculateTaskCost(task1);
    double cost2 = billing->calculateTaskCost(task2);
    billing->processClientPayment(clientId, cost1 + cost2);

    // 4. Submit and Schedule Tasks
    scheduler->submitTask(task1);
    scheduler->submitTask(task2);

    std::cout << "\n--- Starting Task Scheduling ---" << std::endl;
    scheduler->scheduleTasks();
    std::cout << "--- Task Scheduling Complete ---\n" << std::endl;

    // 5. Consensus Verification Demonstration
    std::cout << "--- Running Consensus Verification ---" << std::endl;
    std::vector<std::pair<std::string, std::string>> redundancyResults = {
        {"node-alpha", "SUCCESS_MOCK_RESULT_FOR_task-cpu-01"},
        {"node-beta", "SUCCESS_MOCK_RESULT_FOR_task-cpu-01"}
    };
    std::string verifiedResult;
    verifier->verifyTaskResults(task1, redundancyResults, verifiedResult);
    std::cout << "Verified Task Result: " << verifiedResult << std::endl;

    // 6. Revenue Share & Provider Payout Distribution
    std::cout << "\n--- Distributing Provider Payouts ---" << std::endl;
    std::vector<ProviderMetric> providers = {
        ProviderMetric("node-alpha", 10.0, verifier->getNodeReputation("node-alpha"), "CPU"),
        ProviderMetric("node-beta", 10.0, verifier->getNodeReputation("node-beta"), "GPU_STANDARD")
    };
    billing->distributeProviderPayouts(cost1 + cost2, providers);

    // 7. TCP P2P Networking Demonstration
    std::cout << "\n--- Testing TCP P2P Socket Communications ---" << std::endl;
    TcpServer server;
    server.setMessageHandler([](const std::string& msg) {
        std::cout << "[Server Handler] Received P2P Message: " << msg << std::endl;
        return "NEBULA_P2P_ACK:" + msg;
    });

    int testPort = 19876;
    if (server.start(testPort)) {
        TcpClient client;
        if (client.connectTo("127.0.0.1", testPort)) {
            std::string reply = client.sendAndReceive("SUBMIT_MICRO_TASK:task-999");
            std::cout << "[Client] Received Server Response: " << reply << std::endl;
            client.disconnect();
        }
        server.stop();
    }

    std::cout << "\n==========================================" << std::endl;
    std::cout << " Nebula Compute Pipeline Execution Completed" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}
