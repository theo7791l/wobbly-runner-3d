#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Forward declarations (GLFW sera inclus dans le .cpp)
struct GLFWwindow;

namespace Engine {

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    // Initialisation
    bool Initialize(int width, int height, const std::string& title);
    void Shutdown();
    
    // Boucle de rendu
    void BeginFrame();
    void EndFrame();
    bool ShouldClose() const;
    
    // Primitives de rendu
    void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);
    void DrawSphere(const glm::vec3& position, float radius, const glm::vec3& color);
    void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color);
    
    // Caméra
    void SetCameraPosition(const glm::vec3& position);
    void SetCameraTarget(const glm::vec3& target);
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    
    // Utilitaires
    GLFWwindow* GetWindow() const { return m_window; }
    float GetTime() const;
    
private:
    void SetupOpenGL();
    void CreateShaders();
    void RenderImmediate(); // Mode immediate pour simplicité
    
    GLFWwindow* m_window = nullptr;
    
    // Caméra
    glm::vec3 m_cameraPosition{0.0f, 5.0f, -10.0f};
    glm::vec3 m_cameraTarget{0.0f, 0.0f, 0.0f};
    glm::vec3 m_cameraUp{0.0f, 1.0f, 0.0f};
    
    // Projection
    float m_fov = 60.0f;
    float m_aspectRatio = 16.0f / 9.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 1000.0f;
    
    int m_width = 800;
    int m_height = 600;
};

} // namespace Engine