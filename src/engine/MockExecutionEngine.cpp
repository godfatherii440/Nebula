#include "MockExecutionEngine.h"
#include <iostream>

namespace nebula {

void MockExecutionEngine::initialize() {
    std::cout << "[MockExecutionEngine] Initialized sandbox environment." << std::endl;
}

bool MockExecutionEngine::executeTask(std::shared_ptr<Task> task) {
    std::cout << "[MockExecutionEngine] Executing task: " << task->getId() << std::endl;
    std::cout << "[MockExecutionEngine] Payload: " << task->getPayload() << std::endl;

    // Simulate execution
    std::string mockResult = "SUCCESS_MOCK_RESULT_FOR_" + task->getId();
    task->setResult(mockResult);

    std::cout << "[MockExecutionEngine] Task completed." << std::endl;
    return true;
}

void MockExecutionEngine::shutdown() {
    std::cout << "[MockExecutionEngine] Shutting down sandbox environment." << std::endl;
}

} // namespace nebula
