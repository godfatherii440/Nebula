#ifndef NEBULA_USAGE_BILLING_ENGINE_H
#define NEBULA_USAGE_BILLING_ENGINE_H

#include "nebula/BillingEngine.h"
#include <map>
#include <string>

namespace nebula {

class UsageBillingEngine : public BillingEngine {
private:
    std::map<std::string, double> accounts_;
    double providerShareRatio_; // Default 0.70 (70%)

public:
    UsageBillingEngine(double providerShareRatio = 0.70);
    ~UsageBillingEngine() override = default;

    double calculateTaskCost(std::shared_ptr<Task> task) const override;
    bool processClientPayment(const std::string& clientId, double amount) override;
    std::map<std::string, double> distributeProviderPayouts(
        double grossRevenue,
        const std::vector<ProviderMetric>& providers) override;

    double getAccountBalance(const std::string& accountId) const override;
    void depositFunds(const std::string& accountId, double amount) override;
};

} // namespace nebula

#endif // NEBULA_USAGE_BILLING_ENGINE_H
