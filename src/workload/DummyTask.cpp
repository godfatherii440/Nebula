#include "DummyTask.h"

namespace nebula {

DummyTask::DummyTask(const std::string& id, const std::string& payload)
    : id_(id), payload_(payload), completed_(false) {}

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

} // namespace nebula
