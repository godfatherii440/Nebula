#ifndef NEBULA_MOCK_NODE_AGENT_H
#define NEBULA_MOCK_NODE_AGENT_H

#include "nebula/NodeAgent.h"
#include "nebula/ExecutionEngine.h"
#include <memory>
#include <string>

namespace nebula {

class MockNodeAgent : public NodeAgent {
private:
    std::string nodeId_;
    std::shared_ptr<ExecutionEngine> engine_;
    bool isBusy_;

public:
    MockNodeAgent(const std::string& nodeId, std::shared_ptr<ExecutionEngine> engine);
    ~MockNodeAgent() override;

    std::string getNodeId() const override;
    bool acceptTask(std::shared_ptr<Task> task) override;
    std::string getStatus() const override;
};

} // namespace nebula

#endif // NEBULA_MOCK_NODE_AGENT_H
