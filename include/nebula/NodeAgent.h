#ifndef NEBULA_NODE_AGENT_H
#define NEBULA_NODE_AGENT_H

#include <string>
#include <memory>
#include "nebula/Task.h"
#include "nebula/ExecutionEngine.h"
#include "nebula/ResourceSpec.h"

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
     * @brief Get the hardware resource capacity of the node.
     */
    virtual ResourceSpec getCapacity() const = 0;

    /**
     * @brief Get the node's current reputation score (0.0 to 1.0).
     */
    virtual double getReputationScore() const = 0;

    /**
     * @brief Update the node's reputation score.
     */
    virtual void setReputationScore(double score) = 0;
};

} // namespace nebula

#endif // NEBULA_NODE_AGENT_H
