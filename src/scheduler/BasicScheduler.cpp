#include "BasicScheduler.h"
#include <iostream>

namespace nebula {

void BasicScheduler::registerNode(std::shared_ptr<NodeAgent> node) {
    nodes_.push_back(node);
    std::cout << "[BasicScheduler] Registered node: " << node->getNodeId() << std::endl;
}

void BasicScheduler::submitTask(std::shared_ptr<Task> task) {
    taskQueue_.push(task);
    std::cout << "[BasicScheduler] Submitted task: " << task->getId() << std::endl;
}

void BasicScheduler::scheduleTasks() {
    std::cout << "[BasicScheduler] Starting scheduling loop. Pending tasks: " << taskQueue_.size() << std::endl;

    std::vector<std::pair<std::shared_ptr<Task>, std::string>> processedTasks;

    while (!taskQueue_.empty()) {
        std::shared_ptr<Task> task = taskQueue_.front();
        bool assigned = false;

        // Simple round-robin attempt for available nodes
        static size_t last_node_idx = 0;
        size_t start_idx = last_node_idx;
        do {
            auto& node = nodes_[last_node_idx];
            last_node_idx = (last_node_idx + 1) % nodes_.size();

            if (node->getStatus() == "IDLE") {
                // Check if this node previously failed for this task
                if (failedNodesPerTask_[task->getId()].count(node->getNodeId()) > 0) {
                    continue; // Skip nodes that failed verification for this specific task
                }

                // Check GPU requirement
                if (task->requiresGpu() && !node->hasGpu()) {
                    continue; // Skip this node, it doesn't have a GPU
                }

                std::cout << "[BasicScheduler] Assigning task " << task->getId() << " to node " << node->getNodeId() << std::endl;
                if (node->acceptTask(task)) {
                    assigned = true;
                    processedTasks.push_back({task, node->getNodeId()});
                    taskQueue_.pop();
                    break;
                }
            }
        } while (last_node_idx != start_idx);

        if (!assigned) {
            std::cout << "[BasicScheduler] No available nodes for task " << task->getId() << ". Retrying later." << std::endl;
            break; // Stop scheduling if no nodes are available
        }
    }
    std::cout << "[BasicScheduler] Scheduling loop finished." << std::endl;

    if (verifier_ && paymentManager_) {
        for (const auto& pair : processedTasks) {
            auto task = pair.first;
            auto nodeId = pair.second;

            if (task->isCompleted()) {
                bool isValid = verifier_->verify(task);
                task->setVerificationStatus(isValid);

                if (isValid) {
                    paymentManager_->processPayment(nodeId, task);
                    failedNodesPerTask_.erase(task->getId()); // clear failures on success
                } else {
                    paymentManager_->handlePaymentFailure(nodeId, task);
                    std::cout << "[BasicScheduler] Task " << task->getId() << " failed verification. Re-queuing." << std::endl;
                    failedNodesPerTask_[task->getId()].insert(nodeId); // Track the failure
                    task->reset();
                    submitTask(task);
                }
            }
        }
    }
}

void BasicScheduler::setVerifier(std::shared_ptr<Verifier> verifier) {
    verifier_ = verifier;
}

void BasicScheduler::setPaymentManager(std::shared_ptr<PaymentManager> paymentManager) {
    paymentManager_ = paymentManager;
}

void BasicScheduler::verifyAndProcessTasks() {
    // Deprecated for prototype flow
}

} // namespace nebula
