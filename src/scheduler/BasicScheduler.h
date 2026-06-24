#ifndef NEBULA_BASIC_SCHEDULER_H
#define NEBULA_BASIC_SCHEDULER_H

#include "nebula/Scheduler.h"
#include <vector>
#include <queue>
#include <memory>

namespace nebula {

class BasicScheduler : public Scheduler {
private:
    std::vector<std::shared_ptr<NodeAgent>> nodes_;
    std::queue<std::shared_ptr<Task>> taskQueue_;

public:
    void registerNode(std::shared_ptr<NodeAgent> node) override;
    void submitTask(std::shared_ptr<Task> task) override;
    void scheduleTasks() override;
};

} // namespace nebula

#endif // NEBULA_BASIC_SCHEDULER_H
