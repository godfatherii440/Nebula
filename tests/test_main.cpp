#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <cmath>

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

void test_resource_spec_and_dummy_task() {
    ResourceSpec low(2, 0, 2048, "CPU");
    ResourceSpec high(8, 2, 16384, "GPU_STANDARD");

    assert(low.fitsIn(high));
    assert(!high.fitsIn(low));

    DummyTask task("t1", "payload", low, 2.5);
    assert(task.getId() == "t1");
    assert(task.getPayload() == "payload");
    assert(task.getComputeUnits() == 2.5);
    assert(task.getResourceRequirements().vCPUs == 2);
    assert(!task.isCompleted());

    task.setResult("success");
    assert(task.getResult() == "success");
    assert(task.isCompleted());

    std::cout << "test_resource_spec_and_dummy_task passed!" << std::endl;
}

void test_mock_engine() {
    auto engine = std::make_shared<MockExecutionEngine>();
    engine->initialize();

    auto task = std::make_shared<DummyTask>("t2", "payload2");
    bool success = engine->executeTask(task);

    assert(success);
    assert(task->isCompleted());
    assert(task->getResult() == "SUCCESS_MOCK_RESULT_FOR_t2");

    engine->shutdown();
    std::cout << "test_mock_engine passed!" << std::endl;
}

void test_node_agent_capacity() {
    auto engine = std::make_shared<MockExecutionEngine>();
    ResourceSpec capacity(4, 1, 8192, "GPU_STANDARD");
    MockNodeAgent node("node1", engine, capacity, 0.9);

    assert(node.getNodeId() == "node1");
    assert(node.getStatus() == "IDLE");
    assert(node.getCapacity().vGPUs == 1);
    assert(node.getReputationScore() == 0.9);

    ResourceSpec validReq(2, 1, 4096, "GPU_STANDARD");
    auto validTask = std::make_shared<DummyTask>("t3", "payload3", validReq);
    assert(node.acceptTask(validTask));

    ResourceSpec tooBig(16, 4, 32768, "GPU_HIGH_END");
    auto oversizedTask = std::make_shared<DummyTask>("t4", "payload4", tooBig);
    assert(!node.acceptTask(oversizedTask));

    std::cout << "test_node_agent_capacity passed!" << std::endl;
}

void test_resource_aware_scheduler() {
    auto scheduler = std::make_shared<BasicScheduler>();
    auto engine = std::make_shared<MockExecutionEngine>();

    ResourceSpec cpuOnly(4, 0, 8192, "CPU");
    ResourceSpec gpuCap(8, 2, 16384, "GPU_STANDARD");

    auto nodeCpu = std::make_shared<MockNodeAgent>("n_cpu", engine, cpuOnly);
    auto nodeGpu = std::make_shared<MockNodeAgent>("n_gpu", engine, gpuCap);

    scheduler->registerNode(nodeCpu);
    scheduler->registerNode(nodeGpu);

    ResourceSpec gpuReq(4, 1, 4096, "GPU_STANDARD");
    auto gpuTask = std::make_shared<DummyTask>("gpu_task", "p_gpu", gpuReq);

    scheduler->submitTask(gpuTask);
    scheduler->scheduleTasks();

    assert(gpuTask->isCompleted());
    assert(gpuTask->getResult() == "SUCCESS_MOCK_RESULT_FOR_gpu_task");

    std::cout << "test_resource_aware_scheduler passed!" << std::endl;
}

void test_consensus_verification_engine() {
    ConsensusVerificationEngine verifier;

    auto task = std::make_shared<DummyTask>("v_task", "payload");
    std::vector<std::pair<std::string, std::string>> results = {
        {"node-1", "RESULT_A"},
        {"node-2", "RESULT_A"},
        {"node-3", "RESULT_B"} // Malicious or faulty node
    };

    std::string verifiedResult;
    bool success = verifier.verifyTaskResults(task, results, verifiedResult);

    assert(success);
    assert(verifiedResult == "RESULT_A");
    assert(task->getResult() == "RESULT_A");

    // Correct nodes gain reputation, bad node loses reputation
    assert(verifier.getNodeReputation("node-1") > 1.0 - 0.001); // 1.0 (capped max)
    assert(verifier.getNodeReputation("node-3") < 0.85); // penalized (-0.20)

    std::cout << "test_consensus_verification_engine passed!" << std::endl;
}

void test_usage_billing_engine() {
    UsageBillingEngine billing(0.70);

    billing.depositFunds("client-1", 50.00);
    assert(billing.getAccountBalance("client-1") == 50.00);

    ResourceSpec req(2, 1, 4096, "GPU_STANDARD");
    auto task = std::make_shared<DummyTask>("b_task", "payload", req, 2.0);

    double cost = billing.calculateTaskCost(task); // 2.0 * 0.10 * 2.0 (GPU_STANDARD) = 0.40
    assert(std::fabs(cost - 0.40) < 0.0001);

    bool paid = billing.processClientPayment("client-1", cost);
    assert(paid);
    assert(std::fabs(billing.getAccountBalance("client-1") - 49.60) < 0.0001);

    std::vector<ProviderMetric> providers = {
        ProviderMetric("prov-1", 10.0, 1.0, "GPU_STANDARD"),
        ProviderMetric("prov-2", 10.0, 0.5, "GPU_STANDARD")
    };

    auto payouts = billing.distributeProviderPayouts(10.00, providers); // Gross $10 -> $7 pool
    assert(payouts["prov-1"] > payouts["prov-2"]);
    assert(std::fabs((payouts["prov-1"] + payouts["prov-2"]) - 7.00) < 0.001);

    std::cout << "test_usage_billing_engine passed!" << std::endl;
}

void test_tcp_network_engine() {
    TcpServer server;
    server.setMessageHandler([](const std::string& msg) {
        return "ECHO:" + msg;
    });

    int port = 19888;
    bool started = server.start(port);
    assert(started);

    TcpClient client;
    bool connected = client.connectTo("127.0.0.1", port);
    assert(connected);

    std::string response = client.sendAndReceive("HELLO_NEBULA");
    assert(response == "ECHO:HELLO_NEBULA");

    client.disconnect();
    server.stop();

    std::cout << "test_tcp_network_engine passed!" << std::endl;
}

int main() {
    std::cout << "Running Nebula Compute test suite...\n" << std::endl;

    test_resource_spec_and_dummy_task();
    test_mock_engine();
    test_node_agent_capacity();
    test_resource_aware_scheduler();
    test_consensus_verification_engine();
    test_usage_billing_engine();
    test_tcp_network_engine();

    std::cout << "\nAll Nebula Compute tests passed successfully!" << std::endl;
    return 0;
}
