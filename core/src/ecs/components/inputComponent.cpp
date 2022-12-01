#include <core/ecs/components/inputComponent.hpp>
#include <core/input/inputManager.hpp>

#include <core/utils/logging.hpp>

InputComponent::InputComponent() {}
InputComponent::~InputComponent() = default;

void InputComponent::init() {

}

void InputComponent::setListener(const InputEventType event, const Listener &listener)
{
    L_TAG("InputComponent::setListener");
    this->m_listeners.emplace(event, listener);

    L_TRACE("{}: listener set for {}", static_cast<void*>(this), to_string(event));
}