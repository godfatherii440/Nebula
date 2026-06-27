#ifndef NEBULA_TASK_H
#define NEBULA_TASK_H

#include <string>
#include <memory>
#include "nebula/HardwareRequirements.h"

namespace nebula {

/**
 * @brief Represents a compute micro-task in the Nebula Compute network.
 */
class Task {
public:
    virtual ~Task() = default;

    /**
     * @brief Get the unique identifier for this task.
     */
    virtual std::string getId() const = 0;

    /**
     * @brief Get the payload or instructions for this task.
     */
    virtual std::string getPayload() const = 0;

    /**
     * @brief Set the result of the task execution.
     */
    virtual void setResult(const std::string& result) = 0;

    /**
     * @brief Get the result of the task execution.
     */
    virtual std::string getResult() const = 0;

    /**
     * @brief Check if the task is completed.
     */
    virtual bool isCompleted() const = 0;

    /**
     * @brief Get the hardware requirements (vCPU/vGPU) for this task.
     */
    virtual std::shared_ptr<HardwareRequirements> getHardwareRequirements() const = 0;
};

} // namespace nebula

#endif // NEBULA_TASK_H
