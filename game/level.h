#pragma once

#include "../engine/physics.h"
#include "../engine/renderer.h"
#include <vector>
#include <memory>

namespace Game {

// Type d'obstacle
enum class ObstacleType {
    Platform,
    RotatingBar,
    MovingPlatform,
    Gap,
    Ramp
};

// Un obstacle dans le niveau
struct Obstacle {
    ObstacleType type;
    glm::vec3 position;
    glm::vec3 size;
    Engine::RigidBody* body = nullptr;
    
    // Pour les obstacles animés
    float animationTime = 0.0f;
    float animationSpeed = 1.0f;
};

// Générateur de niveau procédural
class Level {
public:
    explicit Level(Engine::PhysicsEngine* physics);
    ~Level();
    
    // Génération
    void GenerateObstacleCourse(float length);
    void Clear();
    void Reset();
    
    // Mise à jour et rendu
    void Update(float deltaTime);
    void Render(Engine::Renderer* renderer);
    
private:
    void CreateGround();
    void AddPlatform(const glm::vec3& position, const glm::vec3& size);
    void AddRotatingBar(const glm::vec3& position, float length);
    void AddMovingPlatform(const glm::vec3& position, const glm::vec3& size);
    void AddRamp(const glm::vec3& position, const glm::vec3& size);
    
    Engine::PhysicsEngine* m_physics;
    std::vector<Obstacle> m_obstacles;
    Engine::RigidBody* m_ground;
    
    float m_courseLength = 50.0f;
};

} // namespace Game