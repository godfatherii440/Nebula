#ifndef NEBULA_BASIC_SCHEDULER_H
#define NEBULA_BASIC_SCHEDULER_H

#include "nebula/Scheduler.h"
#include <vector>
#include <queue>
#include <memory>
#include <map>
#include <string>
#include <set>

namespace nebula {

class BasicScheduler : public Scheduler {
private:
    std::vector<std::shared_ptr<NodeAgent>> nodes_;
    std::queue<std::shared_ptr<Task>> taskQueue_;
    std::shared_ptr<Verifier> verifier_;
    std::shared_ptr<PaymentManager> paymentManager_;
    std::map<std::string, std::set<std::string>> failedNodesPerTask_; // Task ID -> Set of Node IDs

public:
    void registerNode(std::shared_ptr<NodeAgent> node) override;
    void submitTask(std::shared_ptr<Task> task) override;
    void scheduleTasks() override;

    void setVerifier(std::shared_ptr<Verifier> verifier) override;
    void setPaymentManager(std::shared_ptr<PaymentManager> paymentManager) override;
    void verifyAndProcessTasks();
};

} // namespace nebula

#endif // NEBULA_BASIC_SCHEDULER_H
