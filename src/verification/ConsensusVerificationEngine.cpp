#include "ConsensusVerificationEngine.h"
#include <map>
#include <iostream>
#include <algorithm>

namespace nebula {

bool ConsensusVerificationEngine::verifyTaskResults(
    std::shared_ptr<Task> task,
    const std::vector<std::pair<std::string, std::string>>& nodeResults,
    std::string& consensusResult) {

    if (nodeResults.empty()) {
        std::cerr << "[ConsensusVerificationEngine] No results submitted for task "
                  << (task ? task->getId() : "null") << std::endl;
        return false;
    }

    // Count occurrences of each result
    std::map<std::string, int> resultCounts;
    for (const auto& nr : nodeResults) {
        resultCounts[nr.second]++;
    }

    // Find result with maximum votes
    std::string topResult;
    int maxVotes = 0;
    for (const auto& kv : resultCounts) {
        if (kv.second > maxVotes) {
            maxVotes = kv.second;
            topResult = kv.first;
        }
    }

    // Consensus rule: top result must have majority or equal highest votes
    if (maxVotes > 0) {
        consensusResult = topResult;
        if (task) {
            task->setResult(consensusResult);
        }

        // Update reputation for each participating node
        for (const auto& nr : nodeResults) {
            bool correct = (nr.second == consensusResult);
            recordTaskOutcome(nr.first, correct);
        }

        std::cout << "[ConsensusVerificationEngine] Consensus reached for task "
                  << (task ? task->getId() : "") << " with " << maxVotes << "/" << nodeResults.size()
                  << " votes." << std::endl;
        return true;
    }

    return false;
}

double ConsensusVerificationEngine::getNodeReputation(const std::string& nodeId) const {
    auto it = reputationMap_.find(nodeId);
    if (it != reputationMap_.end()) {
        return it->second;
    }
    return 1.0; // Default initial reputation score
}

void ConsensusVerificationEngine::recordTaskOutcome(const std::string& nodeId, bool correct) {
    double currentRep = getNodeReputation(nodeId);
    if (correct) {
        currentRep = std::min(1.0, currentRep + 0.05);
    } else {
        currentRep = std::max(0.0, currentRep - 0.20);
    }
    reputationMap_[nodeId] = currentRep;
    std::cout << "[ConsensusVerificationEngine] Updated reputation for node " << nodeId
              << ": " << currentRep << std::endl;
}

} // namespace nebula
