#include "MockPaymentManager.h"
#include <iostream>

namespace nebula {

void MockPaymentManager::processPayment(const std::string& nodeId, std::shared_ptr<Task> task) {
    std::cout << "[MockPaymentManager] Processed payment to node " << nodeId
              << " for successful task " << task->getId() << std::endl;
}

void MockPaymentManager::handlePaymentFailure(const std::string& nodeId, std::shared_ptr<Task> task) {
    std::cout << "[MockPaymentManager] Payment failed/penalized for node " << nodeId
              << " due to task " << task->getId() << " failing verification." << std::endl;
}

} // namespace nebula
