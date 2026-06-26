#include "MockVerifier.h"
#include <iostream>

namespace nebula {

bool MockVerifier::verify(std::shared_ptr<Task> task) {
    if (!task->isCompleted()) {
        std::cerr << "[MockVerifier] Cannot verify incomplete task " << task->getId() << std::endl;
        return false;
    }

    std::string result = task->getResult();
    // Simulate verification logic: we fail if result contains "FAIL"
    bool success = result.find("FAIL") == std::string::npos;

    std::cout << "[MockVerifier] Verifying task " << task->getId() << " ... " << (success ? "PASSED" : "FAILED") << std::endl;
    return success;
}

} // namespace nebula
