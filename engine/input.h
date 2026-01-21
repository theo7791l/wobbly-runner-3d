#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Engine {

class InputSystem {
public:
    explicit InputSystem(GLFWwindow* window);
    ~InputSystem();
    
    // Mise à jour
    void Update();
    
    // Queries
    bool IsKeyPressed(int key) const;
    bool IsKeyDown(int key) const;
    bool IsKeyReleased(int key) const;
    
    // Mouse (pour plus tard)
    void GetMousePosition(double& x, double& y) const;
    bool IsMouseButtonPressed(int button) const;
    
private:
    GLFWwindow* m_window;
    
    // États des touches
    std::unordered_map<int, bool> m_keysPressed;
    std::unordered_map<int, bool> m_keysPressedLastFrame;
};

} // namespace Engine