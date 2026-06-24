#ifndef NEBULA_EXECUTION_ENGINE_H
#define NEBULA_EXECUTION_ENGINE_H

#include <memory>
#include "nebula/Task.h"

namespace nebula {

/**
 * @brief Represents the sandbox/execution layer for running tasks.
 */
class ExecutionEngine {
public:
    virtual ~ExecutionEngine() = default;

    /**
     * @brief Initialize the execution engine environment.
     */
    virtual void initialize() = 0;

    /**
     * @brief Execute a given task within the sandbox environment.
     * @param task A shared pointer to the task to execute.
     * @return true if execution was successful, false otherwise.
     */
    virtual bool executeTask(std::shared_ptr<Task> task) = 0;

    /**
     * @brief Terminate the execution engine environment.
     */
    virtual void shutdown() = 0;
};

} // namespace nebula

#endif // NEBULA_EXECUTION_ENGINE_H
