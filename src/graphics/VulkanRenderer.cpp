#include "hiersim/VulkanRenderer.h"
namespace hiersim {
bool VulkanRenderer::initialize(const VulkanContext& ctx) { context_ = ctx; return true; }
void VulkanRenderer::render() {}
void VulkanRenderer::cleanup() { context_.cleanup(); }
}
