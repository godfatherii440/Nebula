#include "UsageBillingEngine.h"
#include <iostream>

namespace nebula {

UsageBillingEngine::UsageBillingEngine(double providerShareRatio)
    : providerShareRatio_(providerShareRatio) {}

double UsageBillingEngine::calculateTaskCost(std::shared_ptr<Task> task) const {
    if (!task) return 0.0;

    double computeUnits = task->getComputeUnits();
    ResourceSpec reqs = task->getResourceRequirements();

    double baseRate = 0.10; // $0.10 per compute unit base rate
    double multiplier = 1.0;

    if (reqs.hardwareClass == "GPU_HIGH_END") {
        multiplier = 3.0;
    } else if (reqs.hardwareClass == "GPU_STANDARD") {
        multiplier = 2.0;
    } else if (reqs.hardwareClass == "CPU") {
        multiplier = 1.0;
    }

    return computeUnits * baseRate * multiplier;
}

bool UsageBillingEngine::processClientPayment(const std::string& clientId, double amount) {
    double currentBalance = getAccountBalance(clientId);
    if (currentBalance < amount) {
        std::cerr << "[UsageBillingEngine] Payment failed: insufficient balance for client "
                  << clientId << " (Balance: $" << currentBalance << ", Required: $" << amount << ")" << std::endl;
        return false;
    }

    accounts_[clientId] = currentBalance - amount;
    std::cout << "[UsageBillingEngine] Processed payment of $" << amount
              << " for client " << clientId << ". Remaining balance: $" << accounts_[clientId] << std::endl;
    return true;
}

std::map<std::string, double> UsageBillingEngine::distributeProviderPayouts(
    double grossRevenue,
    const std::vector<ProviderMetric>& providers) {

    std::map<std::string, double> payouts;
    if (providers.empty() || grossRevenue <= 0.0) {
        return payouts;
    }

    double totalPayoutPool = grossRevenue * providerShareRatio_;

    // Calculate total weighted score for all providers
    double totalWeight = 0.0;
    std::map<std::string, double> providerWeights;

    for (const auto& p : providers) {
        double hwMultiplier = 1.0;
        if (p.hardwareClass == "GPU_HIGH_END") hwMultiplier = 3.0;
        else if (p.hardwareClass == "GPU_STANDARD") hwMultiplier = 2.0;

        double weight = p.computeTimeSeconds * p.reputationScore * hwMultiplier;
        providerWeights[p.providerId] = weight;
        totalWeight += weight;
    }

    if (totalWeight <= 0.0) {
        return payouts;
    }

    for (const auto& p : providers) {
        double share = (providerWeights[p.providerId] / totalWeight) * totalPayoutPool;
        payouts[p.providerId] = share;
        depositFunds(p.providerId, share);

        std::cout << "[UsageBillingEngine] Provider " << p.providerId
                  << " payout: $" << share << " (Reputation: " << p.reputationScore
                  << ", Class: " << p.hardwareClass << ")" << std::endl;
    }

    return payouts;
}

double UsageBillingEngine::getAccountBalance(const std::string& accountId) const {
    auto it = accounts_.find(accountId);
    if (it != accounts_.end()) {
        return it->second;
    }
    return 0.0;
}

void UsageBillingEngine::depositFunds(const std::string& accountId, double amount) {
    accounts_[accountId] += amount;
    std::cout << "[UsageBillingEngine] Deposited $" << amount << " into account " << accountId
              << ". New balance: $" << accounts_[accountId] << std::endl;
}

} // namespace nebula
