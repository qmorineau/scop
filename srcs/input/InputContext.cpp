#include "InputContext.hpp"

InputContext::InputContext()
{
    _activeKeys.reserve(16);
};

InputContext::~InputContext() = default;

void InputContext::pressKey(int key)
{
    if (_keys.test(key))
        return; 
    _keys.set(key, true);
    _activeKeys.push_back(key);
};

void InputContext::releaseKey(int key)
{
    if (!_keys.test(key))
        return; 
    _keys.reset(key);
    _activeKeys.erase(std::remove(_activeKeys.begin(), _activeKeys.end(), key), _activeKeys.end());
};

void InputContext::pressMouse(int key)
{
    if (_mouse.test(key))
        return; 
    _mouse.set(key, true);
}

void InputContext::releaseMouse(int key)
{
    if (!_mouse.test(key))
        return; 
    _mouse.reset(key);
};