#ifndef NEBULA_BILLING_ENGINE_H
#define NEBULA_BILLING_ENGINE_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "nebula/Task.h"

namespace nebula {

/**
 * @brief Represents provider earnings calculation context.
 */
struct ProviderMetric {
    std::string providerId;
    double computeTimeSeconds;
    double reputationScore;
    std::string hardwareClass;

    ProviderMetric(const std::string& id, double computeTime, double reputation, const std::string& hwClass)
        : providerId(id), computeTimeSeconds(computeTime), reputationScore(reputation), hardwareClass(hwClass) {}
};

/**
 * @brief Interface for usage-based billing, payment processing, and provider payout allocations.
 */
class BillingEngine {
public:
    virtual ~BillingEngine() = default;

    /**
     * @brief Calculate cost for executing a given task based on compute units and hardware class.
     */
    virtual double calculateTaskCost(std::shared_ptr<Task> task) const = 0;

    /**
     * @brief Process client payment for task submission.
     */
    virtual bool processClientPayment(const std::string& clientId, double amount) = 0;

    /**
     * @brief Distribute revenue (e.g. 70% share) to providers based on reputation, compute time, and hardware class.
     */
    virtual std::map<std::string, double> distributeProviderPayouts(
        double grossRevenue,
        const std::vector<ProviderMetric>& providers) = 0;

    /**
     * @brief Get account balance for client or provider.
     */
    virtual double getAccountBalance(const std::string& accountId) const = 0;

    /**
     * @brief Deposit funds into account.
     */
    virtual void depositFunds(const std::string& accountId, double amount) = 0;
};

} // namespace nebula

#endif // NEBULA_BILLING_ENGINE_H
