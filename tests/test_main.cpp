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

    auto node1 = std::make_shared<MockNodeAgent>("n1", engine);
    auto node2 = std::make_shared<MockNodeAgent>("n2", engine);

    scheduler->registerNode(node1);
    scheduler->registerNode(node2);

    auto task1 = std::make_shared<DummyTask>("task1", "p1");
    auto task2 = std::make_shared<DummyTask>("task2", "p2");

    scheduler->submitTask(task1);
    scheduler->submitTask(task2);

    assert(!task1->isCompleted());
    assert(!task2->isCompleted());

    scheduler->scheduleTasks();

    assert(task1->isCompleted());
    assert(task2->isCompleted());
    assert(task1->getResult() == "SUCCESS_MOCK_RESULT_FOR_task1");
    assert(task2->getResult() == "SUCCESS_MOCK_RESULT_FOR_task2");

    std::cout << "test_scheduler passed!" << std::endl;
}

int main() {
    std::cout << "Running tests...\n" << std::endl;

    test_dummy_task();
    test_mock_engine();
    test_node_agent();
    test_scheduler();

    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}
