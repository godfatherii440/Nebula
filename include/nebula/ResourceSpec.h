#ifndef NEBULA_RESOURCE_SPEC_H
#define NEBULA_RESOURCE_SPEC_H

#include <string>

namespace nebula {

/**
 * @brief Represents virtual resource specifications (vCPU, vGPU, RAM, hardware class).
 */
struct ResourceSpec {
    int vCPUs;
    int vGPUs;
    int ramMB;
    std::string hardwareClass; // e.g., "CPU", "GPU_STANDARD", "GPU_HIGH_END"

    ResourceSpec(int vcpus = 1, int vgpus = 0, int ram = 1024, const std::string& hwClass = "CPU")
        : vCPUs(vcpus), vGPUs(vgpus), ramMB(ram), hardwareClass(hwClass) {}

    /**
     * @brief Check if these resource requirements fit within the target capacity.
     */
    bool fitsIn(const ResourceSpec& capacity) const {
        return vCPUs <= capacity.vCPUs &&
               vGPUs <= capacity.vGPUs &&
               ramMB <= capacity.ramMB;
    }
};

} // namespace nebula

#endif // NEBULA_RESOURCE_SPEC_H
