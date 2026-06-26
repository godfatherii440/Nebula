#ifndef NEBULA_DUMMY_TASK_H
#define NEBULA_DUMMY_TASK_H

#include "nebula/Task.h"
#include <string>

namespace nebula {

class DummyTask : public Task {
private:
    std::string id_;
    std::string payload_;
    std::string result_;
    bool completed_;
    bool verified_;
    bool failedVerification_;
    bool requiresGpu_;

public:
    DummyTask(const std::string& id, const std::string& payload, bool requiresGpu = false);

    std::string getId() const override;
    std::string getPayload() const override;
    void setResult(const std::string& result) override;
    std::string getResult() const override;
    bool isCompleted() const override;
    bool requiresGpu() const override;

    bool isVerified() const override;
    bool hasFailedVerification() const override;
    void setVerificationStatus(bool verified) override;
    void reset() override;
};

} // namespace nebula

#endif // NEBULA_DUMMY_TASK_H
