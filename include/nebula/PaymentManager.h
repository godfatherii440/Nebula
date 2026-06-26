#ifndef NEBULA_PAYMENT_MANAGER_H
#define NEBULA_PAYMENT_MANAGER_H

#include <memory>
#include <string>
#include "nebula/Task.h"

namespace nebula {

/**
 * @brief Interface for processing payments to nodes for completed tasks.
 */
class PaymentManager {
public:
    virtual ~PaymentManager() = default;

    /**
     * @brief Process payment for a successfully completed and verified task.
     * @param nodeId The ID of the node that executed the task.
     * @param task The task that was executed.
     */
    virtual void processPayment(const std::string& nodeId, std::shared_ptr<Task> task) = 0;

    /**
     * @brief Handle payment failure/penalty for a task that failed verification.
     * @param nodeId The ID of the node that executed the task.
     * @param task The task that failed verification.
     */
    virtual void handlePaymentFailure(const std::string& nodeId, std::shared_ptr<Task> task) = 0;
};

} // namespace nebula

#endif // NEBULA_PAYMENT_MANAGER_H
