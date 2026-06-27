#ifndef NEBULA_HARDWARE_REQUIREMENTS_H
#define NEBULA_HARDWARE_REQUIREMENTS_H

#include <memory>

namespace nebula {

/**
 * @brief Represents virtual CPU (vCPU) capabilities.
 */
class VCPU {
public:
    virtual ~VCPU() = default;

    /**
     * @brief Get the number of virtual cores.
     */
    virtual int getCores() const = 0;

    /**
     * @brief Get the memory size in MB.
     */
    virtual int getMemoryMB() const = 0;
};

/**
 * @brief Represents virtual GPU (vGPU) capabilities.
 */
class VGPU {
public:
    virtual ~VGPU() = default;

    /**
     * @brief Get the number of GPU cores.
     */
    virtual int getCores() const = 0;

    /**
     * @brief Get the VRAM size in MB.
     */
    virtual int getVRAM_MB() const = 0;
};

/**
 * @brief Represents the combined hardware requirements or capabilities.
 */
class HardwareRequirements {
public:
    virtual ~HardwareRequirements() = default;

    /**
     * @brief Get the vCPU requirements/capabilities.
     * @return A shared pointer to the VCPU object, or nullptr if none.
     */
    virtual std::shared_ptr<VCPU> getVCPU() const = 0;

    /**
     * @brief Get the vGPU requirements/capabilities.
     * @return A shared pointer to the VGPU object, or nullptr if none.
     */
    virtual std::shared_ptr<VGPU> getVGPU() const = 0;
};

} // namespace nebula

#endif // NEBULA_HARDWARE_REQUIREMENTS_H
