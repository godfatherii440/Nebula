#include <iostream>
#include <memory>
#include <vector>
#include <cassert>

#include "nebula/Task.h"
#include "nebula/ExecutionEngine.h"
#include "nebula/NodeAgent.h"
#include "nebula/Scheduler.h"

#include "workload/DummyTask.h"
#include "engine/MockExecutionEngine.h"
#include "node_agent/MockNodeAgent.h"
#include "scheduler/BasicScheduler.h"
#include "verifier/MockVerifier.h"
#include "payment/MockPaymentManager.h"
#include "network/UdpNetworkLayer.h"
#include <thread>
#include <chrono>

using namespace nebula;

void test_dummy_task() {
    DummyTask task("t1", "payload");
    assert(task.getId() == "t1");
    assert(task.getPayload() == "payload");
    assert(!task.isCompleted());

    task.setResult("success");
    assert(task.getResult() == "success");
    assert(task.isCompleted());
    std::cout << "test_dummy_task passed!" << std::endl;
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

void test_node_agent() {
    auto engine = std::make_shared<MockExecutionEngine>();
    MockNodeAgent node("node1", engine);

    assert(node.getNodeId() == "node1");
    assert(node.getStatus() == "IDLE");

    auto task = std::make_shared<DummyTask>("t3", "payload3");
    bool accepted = node.acceptTask(task);

    assert(accepted);
    assert(task->isCompleted()); // because acceptTask runs it synchronously in mock
    assert(task->getResult() == "SUCCESS_MOCK_RESULT_FOR_t3");

    std::cout << "test_node_agent passed!" << std::endl;
}

void test_scheduler() {
    auto scheduler = std::make_shared<BasicScheduler>();
    auto engine = std::make_shared<MockExecutionEngine>();
    auto verifier = std::make_shared<MockVerifier>();
    auto paymentManager = std::make_shared<MockPaymentManager>();

    scheduler->setVerifier(verifier);
    scheduler->setPaymentManager(paymentManager);

    auto node1 = std::make_shared<MockNodeAgent>("n1", engine, false);
    auto node2 = std::make_shared<MockNodeAgent>("n2", engine, true);

    scheduler->registerNode(node1);
    scheduler->registerNode(node2);

    auto task1 = std::make_shared<DummyTask>("task1", "p1", false);
    auto task2 = std::make_shared<DummyTask>("task2", "p2", true);
    auto task3 = std::make_shared<DummyTask>("task_FAIL", "p3", false);

    scheduler->submitTask(task1);
    scheduler->submitTask(task2);
    scheduler->submitTask(task3);

    assert(!task1->isCompleted());
    assert(!task2->isCompleted());

    scheduler->scheduleTasks();

    assert(task1->isCompleted());
    assert(task2->isCompleted());
    assert(task1->getResult() == "SUCCESS_MOCK_RESULT_FOR_task1");
    assert(task2->getResult() == "SUCCESS_MOCK_RESULT_FOR_task2");
    assert(task1->isVerified());
    assert(task2->isVerified());

    // For task3, the mock execution engine will set result to "SUCCESS_MOCK_RESULT_FOR_task_FAIL"
    // Wait, the verifier fails if result contains "FAIL".
    // "SUCCESS_MOCK_RESULT_FOR_task_FAIL" contains "FAIL".
    // So the verifier will return false, the payment will fail, the task will be reset and re-queued.
    // It should now be back in the queue and not completed.
    assert(!task3->isCompleted());

    std::cout << "test_scheduler passed!" << std::endl;
}

void test_network_layer() {
    UdpNetworkLayer net1(8081);
    UdpNetworkLayer net2(8081);

    bool received = false;
    net1.setReceiveCallback([&](const std::string& msg) {
        if (msg == "test_broadcast") received = true;
    });

    net1.start();
    net2.start();

    net2.broadcast("test_broadcast");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    net1.stop();
    net2.stop();

    assert(received);
    std::cout << "test_network_layer passed!" << std::endl;
}

int main() {
    std::cout << "Running tests...\n" << std::endl;

    test_dummy_task();
    test_mock_engine();
    test_node_agent();
    test_scheduler();
    test_network_layer();

    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}
