#ifndef NEBULA_MOCK_NODE_AGENT_H
#define NEBULA_MOCK_NODE_AGENT_H

#include "nebula/NodeAgent.h"
#include "nebula/ExecutionEngine.h"
#include "nebula/ResourceSpec.h"
#include <memory>
#include <string>

namespace nebula {

class MockNodeAgent : public NodeAgent {
private:
    std::string nodeId_;
    std::shared_ptr<ExecutionEngine> engine_;
    bool isBusy_;
    ResourceSpec capacity_;
    double reputationScore_;

public:
    MockNodeAgent(const std::string& nodeId,
                  std::shared_ptr<ExecutionEngine> engine,
                  const ResourceSpec& capacity = ResourceSpec(4, 1, 8192, "GPU_STANDARD"),
                  double reputation = 1.0);
    ~MockNodeAgent() override;

    std::string getNodeId() const override;
    bool acceptTask(std::shared_ptr<Task> task) override;
    std::string getStatus() const override;
    ResourceSpec getCapacity() const override;
    double getReputationScore() const override;
    void setReputationScore(double score) override;
};

} // namespace nebula

#endif // NEBULA_MOCK_NODE_AGENT_H
