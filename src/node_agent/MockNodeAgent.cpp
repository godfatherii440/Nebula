#include "MockNodeAgent.h"
#include <iostream>

namespace nebula {

MockNodeAgent::MockNodeAgent(const std::string& nodeId,
                             std::shared_ptr<ExecutionEngine> engine,
                             const ResourceSpec& capacity,
                             double reputation)
    : nodeId_(nodeId), engine_(engine), isBusy_(false), capacity_(capacity), reputationScore_(reputation) {
    if (engine_) {
        engine_->initialize();
    }
}

MockNodeAgent::~MockNodeAgent() {
    if (engine_) {
        engine_->shutdown();
    }
}

std::string MockNodeAgent::getNodeId() const {
    return nodeId_;
}

bool MockNodeAgent::acceptTask(std::shared_ptr<Task> task) {
    if (isBusy_) {
        std::cout << "[MockNodeAgent " << nodeId_ << "] Busy, rejecting task " << task->getId() << std::endl;
        return false;
    }

    if (!task->getResourceRequirements().fitsIn(capacity_)) {
        std::cout << "[MockNodeAgent " << nodeId_ << "] Insufficient capacity for task " << task->getId() << std::endl;
        return false;
    }

    std::cout << "[MockNodeAgent " << nodeId_ << "] Accepting task " << task->getId() << std::endl;
    isBusy_ = true;

    if (engine_) {
        engine_->executeTask(task);
    } else {
        std::cerr << "[MockNodeAgent " << nodeId_ << "] Error: No execution engine attached!" << std::endl;
        task->setResult("ERROR_NO_ENGINE");
    }

    isBusy_ = false;
    return true;
}

std::string MockNodeAgent::getStatus() const {
    return isBusy_ ? "BUSY" : "IDLE";
}

ResourceSpec MockNodeAgent::getCapacity() const {
    return capacity_;
}

double MockNodeAgent::getReputationScore() const {
    return reputationScore_;
}

void MockNodeAgent::setReputationScore(double score) {
    reputationScore_ = score;
}

} // namespace nebula
