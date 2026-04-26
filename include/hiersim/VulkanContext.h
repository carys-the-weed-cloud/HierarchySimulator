#pragma once
#include <vulkan/vulkan.h>
namespace hiersim {
class VulkanContext {
public:
    bool initialize();
    void cleanup();
    VkInstance getInstance() const { return instance_; }
private:
    VkInstance instance_ = VK_NULL_HANDLE;
};
}
