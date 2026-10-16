#ifndef NEBULA_VERIFICATION_ENGINE_H
#define NEBULA_VERIFICATION_ENGINE_H

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "nebula/Task.h"

namespace nebula {

/**
 * @brief Interface for redundant execution and consensus-based task verification.
 */
class VerificationEngine {
public:
    virtual ~VerificationEngine() = default;

    /**
     * @brief Verify task execution results from multiple independent nodes.
     * @param task Shared pointer to the task.
     * @param nodeResults Vector of (nodeId, resultString) pairs.
     * @param consensusResult Output parameter containing the verified consensus result.
     * @return true if consensus was reached, false otherwise.
     */
    virtual bool verifyTaskResults(
        std::shared_ptr<Task> task,
        const std::vector<std::pair<std::string, std::string>>& nodeResults,
        std::string& consensusResult) = 0;

    /**
     * @brief Get current reputation score for a node.
     */
    virtual double getNodeReputation(const std::string& nodeId) const = 0;

    /**
     * @brief Update node reputation based on task verification outcome.
     */
    virtual void recordTaskOutcome(const std::string& nodeId, bool correct) = 0;
};

} // namespace nebula

#endif // NEBULA_VERIFICATION_ENGINE_H
