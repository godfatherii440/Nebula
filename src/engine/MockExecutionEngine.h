#ifndef NEBULA_MOCK_EXECUTION_ENGINE_H
#define NEBULA_MOCK_EXECUTION_ENGINE_H

#include "nebula/ExecutionEngine.h"

namespace nebula {

class MockExecutionEngine : public ExecutionEngine {
public:
    void initialize() override;
    bool executeTask(std::shared_ptr<Task> task) override;
    void shutdown() override;
};

} // namespace nebula

#endif // NEBULA_MOCK_EXECUTION_ENGINE_H
