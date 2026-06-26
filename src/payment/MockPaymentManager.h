#ifndef NEBULA_MOCK_PAYMENT_MANAGER_H
#define NEBULA_MOCK_PAYMENT_MANAGER_H

#include "nebula/PaymentManager.h"

namespace nebula {

class MockPaymentManager : public PaymentManager {
public:
    void processPayment(const std::string& nodeId, std::shared_ptr<Task> task) override;
    void handlePaymentFailure(const std::string& nodeId, std::shared_ptr<Task> task) override;
};

} // namespace nebula

#endif // NEBULA_MOCK_PAYMENT_MANAGER_H
