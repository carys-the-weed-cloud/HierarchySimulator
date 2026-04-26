#pragma once
#include "VulkanContext.h"
namespace hiersim {
class VulkanRenderer {
public:
    bool initialize(const VulkanContext& ctx);
    void render();
    void cleanup();
private:
    VulkanContext context_;
};
}
