#include "DummyTask.h"

namespace nebula {

DummyTask::DummyTask(const std::string& id,
                       const std::string& payload,
                       const ResourceSpec& reqs,
                       double computeUnits)
    : id_(id), payload_(payload), completed_(false), resourceReqs_(reqs), computeUnits_(computeUnits) {}

std::string DummyTask::getId() const {
    return id_;
}

std::string DummyTask::getPayload() const {
    return payload_;
}

void DummyTask::setResult(const std::string& result) {
    result_ = result;
    completed_ = true;
}

std::string DummyTask::getResult() const {
    return result_;
}

bool DummyTask::isCompleted() const {
    return completed_;
}

ResourceSpec DummyTask::getResourceRequirements() const {
    return resourceReqs_;
}

double DummyTask::getComputeUnits() const {
    return computeUnits_;
}

} // namespace nebula
