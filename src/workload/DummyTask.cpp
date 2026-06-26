#include "DummyTask.h"

namespace nebula {

DummyTask::DummyTask(const std::string& id, const std::string& payload, bool requiresGpu)
    : id_(id), payload_(payload), completed_(false), verified_(false), failedVerification_(false), requiresGpu_(requiresGpu) {}

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

bool DummyTask::requiresGpu() const {
    return requiresGpu_;
}

bool DummyTask::isVerified() const {
    return verified_;
}

bool DummyTask::hasFailedVerification() const {
    return failedVerification_;
}

void DummyTask::setVerificationStatus(bool verified) {
    if (verified) {
        verified_ = true;
        failedVerification_ = false;
    } else {
        verified_ = false;
        failedVerification_ = true;
    }
}

void DummyTask::reset() {
    result_ = "";
    completed_ = false;
    verified_ = false;
    failedVerification_ = false;
}

} // namespace nebula
