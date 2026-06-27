#ifndef NEBULA_EXECUTION_ENGINE_H
#define NEBULA_EXECUTION_ENGINE_H

#include <memory>
#include "nebula/Task.h"

namespace nebula {

/**
 * @brief Represents the WebAssembly (WASM) execution layer for running tasks deterministically.
 */
class ExecutionEngine {
public:
    virtual ~ExecutionEngine() = default;

    /**
     * @brief Initialize the WAMR (WebAssembly Micro Runtime) environment.
     */
    virtual void initialize() = 0;

    /**
     * @brief Execute a given WASM task within the sandboxed environment.
     * @param task A shared pointer to the task containing WASM bytecode.
     * @return true if execution was successful, false otherwise.
     */
    virtual bool executeTask(std::shared_ptr<Task> task) = 0;

    /**
     * @brief Terminate the WAMR environment.
     */
    virtual void shutdown() = 0;
};

} // namespace nebula

#endif // NEBULA_EXECUTION_ENGINE_H
