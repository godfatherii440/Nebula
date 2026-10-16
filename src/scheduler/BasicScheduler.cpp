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

    while (!taskQueue_.empty()) {
        std::shared_ptr<Task> task = taskQueue_.front();
        bool assigned = false;

        // Round-robin attempt for available nodes matching task resource requirements
        static size_t last_node_idx = 0;
        size_t start_idx = last_node_idx;
        do {
            if (nodes_.empty()) break;

            auto& node = nodes_[last_node_idx];
            last_node_idx = (last_node_idx + 1) % nodes_.size();

            if (node->getStatus() == "IDLE" && task->getResourceRequirements().fitsIn(node->getCapacity())) {
                std::cout << "[BasicScheduler] Assigning task " << task->getId() << " to node " << node->getNodeId() << std::endl;
                if (node->acceptTask(task)) {
                    assigned = true;
                    taskQueue_.pop();
                    break;
                }
            }
        } while (last_node_idx != start_idx);

        if (!assigned) {
            std::cout << "[BasicScheduler] No available matching nodes for task " << task->getId() << ". Retrying later." << std::endl;
            break; // Stop scheduling if no suitable nodes are currently available
        }
    }
    std::cout << "[BasicScheduler] Scheduling loop finished." << std::endl;
}

} // namespace nebula
