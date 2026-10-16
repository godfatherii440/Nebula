#ifndef NEBULA_DUMMY_TASK_H
#define NEBULA_DUMMY_TASK_H

#include "nebula/Task.h"
#include "nebula/ResourceSpec.h"
#include <string>

namespace nebula {

class DummyTask : public Task {
private:
    std::string id_;
    std::string payload_;
    std::string result_;
    bool completed_;
    ResourceSpec resourceReqs_;
    double computeUnits_;

public:
    DummyTask(const std::string& id,
              const std::string& payload,
              const ResourceSpec& reqs = ResourceSpec(1, 0, 1024, "CPU"),
              double computeUnits = 1.0);

    std::string getId() const override;
    std::string getPayload() const override;
    void setResult(const std::string& result) override;
    std::string getResult() const override;
    bool isCompleted() const override;
    ResourceSpec getResourceRequirements() const override;
    double getComputeUnits() const override;
};

} // namespace nebula

#endif // NEBULA_DUMMY_TASK_H
