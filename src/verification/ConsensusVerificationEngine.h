#ifndef NEBULA_CONSENSUS_VERIFICATION_ENGINE_H
#define NEBULA_CONSENSUS_VERIFICATION_ENGINE_H

#include "nebula/VerificationEngine.h"
#include <map>
#include <string>
#include <vector>

namespace nebula {

class ConsensusVerificationEngine : public VerificationEngine {
private:
    std::map<std::string, double> reputationMap_;

public:
    ConsensusVerificationEngine() = default;
    ~ConsensusVerificationEngine() override = default;

    bool verifyTaskResults(
        std::shared_ptr<Task> task,
        const std::vector<std::pair<std::string, std::string>>& nodeResults,
        std::string& consensusResult) override;

    double getNodeReputation(const std::string& nodeId) const override;
    void recordTaskOutcome(const std::string& nodeId, bool correct) override;
};

} // namespace nebula

#endif // NEBULA_CONSENSUS_VERIFICATION_ENGINE_H
