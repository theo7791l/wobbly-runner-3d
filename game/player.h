#pragma once

#include "../engine/physics.h"
#include "../engine/renderer.h"
#include <glm/glm.hpp>
#include <vector>

namespace Game {

// Le personnage ragdoll avec physique
class Player {
public:
    explicit Player(Engine::PhysicsEngine* physics);
    ~Player();
    
    // Commandes
    void LiftLeftLeg();
    void LiftRightLeg();
    void LeanForward();
    void LeanBackward();
    void Jump();
    
    // Mise à jour et rendu
    void Update(float deltaTime);
    void Render(Engine::Renderer* renderer);
    
    // Position
    glm::vec3 GetPosition() const;
    void Reset();
    
private:
    void CreateRagdoll();
    void ApplyMovementForce(const glm::vec3& force);
    
    Engine::PhysicsEngine* m_physics;
    
    // Parties du corps (ragdoll)
    Engine::RigidBody* m_head;
    Engine::RigidBody* m_torso;
    Engine::RigidBody* m_pelvis;
    Engine::RigidBody* m_leftThigh;
    Engine::RigidBody* m_rightThigh;
    Engine::RigidBody* m_leftCalf;
    Engine::RigidBody* m_rightCalf;
    Engine::RigidBody* m_leftArm;
    Engine::RigidBody* m_rightArm;
    
    std::vector<Engine::RigidBody*> m_bodyParts;
    
    // Position initiale
    glm::vec3 m_startPosition{0.0f, 3.0f, 0.0f};
    
    // Cooldowns pour les mouvements
    float m_leftLegCooldown = 0.0f;
    float m_rightLegCooldown = 0.0f;
    float m_jumpCooldown = 0.0f;
};

} // namespace Game