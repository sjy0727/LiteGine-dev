#include "Layer.h"
#include "hzpch.h"

namespace Hazel
{

    Layer::Layer(const std::string& name) : m_DebugName(name) {}

    Layer::~Layer() = default;

} // namespace Hazel