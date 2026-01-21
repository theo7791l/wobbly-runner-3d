#include "level.h"
#include <iostream>
#include <random>

namespace Game {

Level::Level(Engine::PhysicsEngine* physics)
    : m_physics(physics) {
    CreateGround();
}

Level::~Level() {
    Clear();
}

void Level::CreateGround() {
    m_ground = m_physics->CreateRigidBody();
    m_ground->position = glm::vec3(0.0f, -0.5f, 25.0f);
    m_ground->boxMin = glm::vec3(-10.0f, -0.5f, -25.0f);
    m_ground->boxMax = glm::vec3(10.0f, 0.5f, 25.0f);
    m_ground->isKinematic = true;
    m_ground->useGravity = false;
}

void Level::GenerateObstacleCourse(float length) {
    Clear();
    m_courseLength = length;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> obstacleDist(0, 4);
    
    float currentZ = 5.0f; // Commencer après le spawn
    int obstacleCount = 0;
    
    std::cout << "🏭 Génération du parcours d'obstacles..." << std::endl;
    
    while (currentZ < length - 5.0f) {
        ObstacleType type = static_cast<ObstacleType>(obstacleDist(gen));
        
        switch (type) {
            case ObstacleType::Platform:
                AddPlatform(glm::vec3(0.0f, 0.0f, currentZ), glm::vec3(3.0f, 0.3f, 2.0f));
                currentZ += 3.0f;
                break;
                
            case ObstacleType::RotatingBar:
                AddRotatingBar(glm::vec3(0.0f, 2.0f, currentZ), 4.0f);
                currentZ += 4.0f;
                break;
                
            case ObstacleType::MovingPlatform:
                AddMovingPlatform(glm::vec3(0.0f, 0.5f, currentZ), glm::vec3(2.5f, 0.3f, 2.0f));
                currentZ += 3.5f;
                break;
                
            case ObstacleType::Gap:
                // Trou dans le parcours (pas d'obstacle)
                currentZ += 4.0f;
                break;
                
            case ObstacleType::Ramp:
                AddRamp(glm::vec3(0.0f, 0.0f, currentZ), glm::vec3(3.0f, 1.5f, 3.0f));
                currentZ += 4.0f;
                break;
        }
        
        obstacleCount++;
    }
    
    // Ligne d'arrivée
    AddPlatform(glm::vec3(0.0f, 0.0f, length), glm::vec3(5.0f, 0.5f, 3.0f));
    
    std::cout << "✅ Parcours généré : " << obstacleCount << " obstacles sur " 
              << static_cast<int>(length) << "m" << std::endl;
}

void Level::AddPlatform(const glm::vec3& position, const glm::vec3& size) {
    Obstacle obstacle;
    obstacle.type = ObstacleType::Platform;
    obstacle.position = position;
    obstacle.size = size;
    
    obstacle.body = m_physics->CreateRigidBody();
    obstacle.body->position = position;
    obstacle.body->boxMin = -size * 0.5f;
    obstacle.body->boxMax = size * 0.5f;
    obstacle.body->isKinematic = true;
    obstacle.body->useGravity = false;
    
    m_obstacles.push_back(obstacle);
}

void Level::AddRotatingBar(const glm::vec3& position, float length) {
    Obstacle obstacle;
    obstacle.type = ObstacleType::RotatingBar;
    obstacle.position = position;
    obstacle.size = glm::vec3(length, 0.3f, 0.3f);
    obstacle.animationSpeed = 1.5f;
    
    obstacle.body = m_physics->CreateRigidBody();
    obstacle.body->position = position;
    obstacle.body->boxMin = glm::vec3(-length * 0.5f, -0.15f, -0.15f);
    obstacle.body->boxMax = glm::vec3(length * 0.5f, 0.15f, 0.15f);
    obstacle.body->isKinematic = true;
    obstacle.body->useGravity = false;
    
    m_obstacles.push_back(obstacle);
}

void Level::AddMovingPlatform(const glm::vec3& position, const glm::vec3& size) {
    Obstacle obstacle;
    obstacle.type = ObstacleType::MovingPlatform;
    obstacle.position = position;
    obstacle.size = size;
    obstacle.animationSpeed = 0.8f;
    
    obstacle.body = m_physics->CreateRigidBody();
    obstacle.body->position = position;
    obstacle.body->boxMin = -size * 0.5f;
    obstacle.body->boxMax = size * 0.5f;
    obstacle.body->isKinematic = true;
    obstacle.body->useGravity = false;
    
    m_obstacles.push_back(obstacle);
}

void Level::AddRamp(const glm::vec3& position, const glm::vec3& size) {
    Obstacle obstacle;
    obstacle.type = ObstacleType::Ramp;
    obstacle.position = position;
    obstacle.size = size;
    
    obstacle.body = m_physics->CreateRigidBody();
    obstacle.body->position = position;
    obstacle.body->boxMin = -size * 0.5f;
    obstacle.body->boxMax = size * 0.5f;
    obstacle.body->isKinematic = true;
    obstacle.body->useGravity = false;
    
    m_obstacles.push_back(obstacle);
}

void Level::Update(float deltaTime) {
    for (auto& obstacle : m_obstacles) {
        obstacle.animationTime += deltaTime * obstacle.animationSpeed;
        
        // Animer les obstacles
        switch (obstacle.type) {
            case ObstacleType::RotatingBar:
                // Rotation autour de l'axe Y
                if (obstacle.body) {
                    float angle = obstacle.animationTime;
                    float radius = obstacle.size.x * 0.5f;
                    // Simulation simple de rotation (juste pour le visuel)
                }
                break;
                
            case ObstacleType::MovingPlatform:
                // Mouvement de gauche à droite
                if (obstacle.body) {
                    float offset = std::sin(obstacle.animationTime) * 3.0f;
                    obstacle.body->position.x = obstacle.position.x + offset;
                }
                break;
                
            default:
                break;
        }
    }
}

void Level::Render(Engine::Renderer* renderer) {
    // Dessiner le sol
    glm::vec3 groundSize = m_ground->boxMax - m_ground->boxMin;
    renderer->DrawCube(m_ground->position, groundSize, glm::vec3(0.3f, 0.7f, 0.3f));
    
    // Dessiner les obstacles
    for (const auto& obstacle : m_obstacles) {
        glm::vec3 color;
        
        switch (obstacle.type) {
            case ObstacleType::Platform:
                color = glm::vec3(0.6f, 0.4f, 0.2f); // Marron
                break;
            case ObstacleType::RotatingBar:
                color = glm::vec3(0.9f, 0.2f, 0.2f); // Rouge (danger!)
                break;
            case ObstacleType::MovingPlatform:
                color = glm::vec3(0.8f, 0.6f, 0.2f); // Orange
                break;
            case ObstacleType::Ramp:
                color = glm::vec3(0.5f, 0.5f, 0.5f); // Gris
                break;
            default:
                color = glm::vec3(0.5f, 0.5f, 0.5f);
                break;
        }
        
        if (obstacle.body) {
            glm::vec3 size = obstacle.body->boxMax - obstacle.body->boxMin;
            renderer->DrawCube(obstacle.body->position, size, color);
        }
    }
    
    // Dessiner la ligne d'arrivée (dernière plateforme en vert)
    if (!m_obstacles.empty()) {
        const auto& finish = m_obstacles.back();
        if (finish.body) {
            glm::vec3 size = finish.body->boxMax - finish.body->boxMin;
            renderer->DrawCube(finish.body->position, size, glm::vec3(0.2f, 0.9f, 0.2f));
        }
    }
}

void Level::Clear() {
    m_obstacles.clear();
}

void Level::Reset() {
    // Régénérer le même parcours
    GenerateObstacleCourse(m_courseLength);
}

} // namespace Game