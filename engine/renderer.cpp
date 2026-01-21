#include "renderer.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine {

Renderer::Renderer() {}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize(int width, int height, const std::string& title) {
    m_width = width;
    m_height = height;
    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    
    // Initialiser GLFW
    if (!glfwInit()) {
        std::cerr << "Erreur: Impossible d'initialiser GLFW" << std::endl;
        return false;
    }
    
    // Configuration OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // MSAA
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    // Créer la fenêtre
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Erreur: Impossible de créer la fenêtre GLFW" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // VSync
    
    // Initialiser GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Erreur GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    
    SetupOpenGL();
    
    std::cout << "✅ Renderer initialisé (OpenGL " << glGetString(GL_VERSION) << ")" << std::endl;
    return true;
}

void Renderer::SetupOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Bleu ciel
}

void Renderer::Shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void Renderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Setup matrices
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&GetProjectionMatrix()[0][0]);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&GetViewMatrix()[0][0]);
}

void Renderer::EndFrame() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool Renderer::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color) {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    
    glColor3f(color.r, color.g, color.b);
    
    // Dessin simple d'un cube (mode immediate)
    glBegin(GL_QUADS);
    
    // Face avant
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    
    // Face arrière
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    
    // Face gauche
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    
    // Face droite
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    
    // Face dessus
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    
    // Face dessous
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    
    glEnd();
    
    glPopMatrix();
}

void Renderer::DrawSphere(const glm::vec3& position, float radius, const glm::vec3& color) {
    // Simplification: dessiner une sphère avec des cubes (icosphère basique)
    DrawCube(position, glm::vec3(radius * 2.0f), color);
}

void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {
    glColor3f(color.r, color.g, color.b);
    glLineWidth(2.0f);
    
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
}

void Renderer::SetCameraPosition(const glm::vec3& position) {
    m_cameraPosition = position;
}

void Renderer::SetCameraTarget(const glm::vec3& target) {
    m_cameraTarget = target;
}

glm::mat4 Renderer::GetViewMatrix() const {
    return glm::lookAt(m_cameraPosition, m_cameraTarget, m_cameraUp);
}

glm::mat4 Renderer::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}

float Renderer::GetTime() const {
    return static_cast<float>(glfwGetTime());
}

} // namespace Engine
