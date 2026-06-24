#include <iostream>
#include <memory>
#include <vector>

#include "nebula/Task.h"
#include "nebula/ExecutionEngine.h"
#include "nebula/NodeAgent.h"
#include "nebula/Scheduler.h"

// We need the mock headers for instantiation
#include "workload/DummyTask.h"
#include "engine/MockExecutionEngine.h"
#include "node_agent/MockNodeAgent.h"
#include "scheduler/BasicScheduler.h"

using namespace nebula;

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << " Nebula Compute - MVP Prototype v0.1      " << std::endl;
    std::cout << "==========================================" << std::endl;

    // 1. Create the scheduler
    auto scheduler = std::make_shared<BasicScheduler>();

    // 2. Setup the execution engine
    auto engine = std::make_shared<MockExecutionEngine>();

    // 3. Create node agents and register them with the scheduler
    auto node1 = std::make_shared<MockNodeAgent>("node-alpha", engine);
    auto node2 = std::make_shared<MockNodeAgent>("node-beta", engine);

    scheduler->registerNode(node1);
    scheduler->registerNode(node2);

    // 4. Create some tasks and submit them
    std::vector<std::shared_ptr<Task>> tasks;
    tasks.push_back(std::make_shared<DummyTask>("task-001", "CPU_WORKLOAD_A"));
    tasks.push_back(std::make_shared<DummyTask>("task-002", "CPU_WORKLOAD_B"));
    tasks.push_back(std::make_shared<DummyTask>("task-003", "CPU_WORKLOAD_C"));

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
            std::cout << "Completed: " << task->getResult() << std::endl;
        } else {
            std::cout << "Pending" << std::endl;
        }
    }

    return 0;
}
