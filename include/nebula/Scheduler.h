#ifndef NEBULA_SCHEDULER_H
#define NEBULA_SCHEDULER_H

#include <memory>
#include <vector>
#include "nebula/Task.h"
#include "nebula/NodeAgent.h"
#include "nebula/HardwareRequirements.h"

namespace nebula {

/**
 * @brief Represents the centralized or decentralized scheduler.
 */
class Scheduler {
public:
    virtual ~Scheduler() = default;

    /**
     * @brief Register a new compute node to the scheduler.
     * @param node A shared pointer to the NodeAgent.
     */
    virtual void registerNode(std::shared_ptr<NodeAgent> node) = 0;

    /**
     * @brief Submit a task to the scheduler for execution.
     * @param task A shared pointer to the Task.
     */
    virtual void submitTask(std::shared_ptr<Task> task) = 0;

    /**
     * @brief Perform the scheduling loop (assign queued tasks to idle nodes based on hardware requirements).
     */
    virtual void scheduleTasks() = 0;
};

} // namespace nebula

#endif // NEBULA_SCHEDULER_H
