#ifndef NEBULA_MOCK_VERIFIER_H
#define NEBULA_MOCK_VERIFIER_H

#include "nebula/Verifier.h"

namespace nebula {

class MockVerifier : public Verifier {
public:
    bool verify(std::shared_ptr<Task> task) override;
};

} // namespace nebula

#endif // NEBULA_MOCK_VERIFIER_H
