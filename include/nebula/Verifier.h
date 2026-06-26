#ifndef NEBULA_VERIFIER_H
#define NEBULA_VERIFIER_H

#include <memory>
#include "nebula/Task.h"

namespace nebula {

/**
 * @brief Interface for automated verification of task results.
 */
class Verifier {
public:
    virtual ~Verifier() = default;

    /**
     * @brief Verify the result of a completed task.
     * @param task A shared pointer to the completed task.
     * @return true if the result is correct, false otherwise.
     */
    virtual bool verify(std::shared_ptr<Task> task) = 0;
};

} // namespace nebula

#endif // NEBULA_VERIFIER_H
