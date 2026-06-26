#ifndef NEBULA_NODE_AGENT_H
#define NEBULA_NODE_AGENT_H

#include <string>
#include <memory>
#include "nebula/Task.h"
#include "nebula/ExecutionEngine.h"

namespace nebula {

/**
 * @brief Represents a compute node in the Nebula network.
 */
class NodeAgent {
public:
    virtual ~NodeAgent() = default;

    /**
     * @brief Get the unique identifier for this node.
     */
    virtual std::string getNodeId() const = 0;

    /**
     * @brief Accept a task for execution.
     * @param task A shared pointer to the task to execute.
     * @return true if the node accepts the task, false if busy or unavailable.
     */
    virtual bool acceptTask(std::shared_ptr<Task> task) = 0;

    /**
     * @brief Get the current status or capacity of the node.
     * @return A string representing status (e.g., "IDLE", "BUSY").
     */
    virtual std::string getStatus() const = 0;

    /**
     * @brief Check if the node has a GPU available.
     */
    virtual bool hasGpu() const = 0;
};

} // namespace nebula

#endif // NEBULA_NODE_AGENT_H
