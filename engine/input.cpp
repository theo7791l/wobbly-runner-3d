#include "input.h"

namespace Engine {

InputSystem::InputSystem(GLFWwindow* window)
    : m_window(window) {}

InputSystem::~InputSystem() {}

void InputSystem::Update() {
    // Sauvegarder l'état précédent
    m_keysPressedLastFrame = m_keysPressed;
    
    // Mise à jour de l'état actuel
    m_keysPressed.clear();
    
    // Vérifier les touches importantes
    int keys[] = {
        GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D,
        GLFW_KEY_Q, GLFW_KEY_E, GLFW_KEY_Z, GLFW_KEY_X,
        GLFW_KEY_SPACE, GLFW_KEY_ESCAPE, GLFW_KEY_R,
        GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL
    };
    
    for (int key : keys) {
        m_keysPressed[key] = (glfwGetKey(m_window, key) == GLFW_PRESS);
    }
    
    // ESC pour fermer
    if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

bool InputSystem::IsKeyPressed(int key) const {
    auto it = m_keysPressed.find(key);
    return it != m_keysPressed.end() && it->second;
}

bool InputSystem::IsKeyDown(int key) const {
    // Appuyé cette frame mais pas la précédente
    auto itCurrent = m_keysPressed.find(key);
    auto itLast = m_keysPressedLastFrame.find(key);
    
    bool currentPressed = (itCurrent != m_keysPressed.end() && itCurrent->second);
    bool lastPressed = (itLast != m_keysPressedLastFrame.end() && itLast->second);
    
    return currentPressed && !lastPressed;
}

bool InputSystem::IsKeyReleased(int key) const {
    // Relâché cette frame mais appuyé la précédente
    auto itCurrent = m_keysPressed.find(key);
    auto itLast = m_keysPressedLastFrame.find(key);
    
    bool currentPressed = (itCurrent != m_keysPressed.end() && itCurrent->second);
    bool lastPressed = (itLast != m_keysPressedLastFrame.end() && itLast->second);
    
    return !currentPressed && lastPressed;
}

void InputSystem::GetMousePosition(double& x, double& y) const {
    glfwGetCursorPos(m_window, &x, &y);
}

bool InputSystem::IsMouseButtonPressed(int button) const {
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

} // namespace Engine