#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace Engine {

// Représente un corps rigide dans le monde physique
struct RigidBody {
    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};
    glm::vec3 force{0.0f};
    
    float mass = 1.0f;
    float friction = 0.3f;
    float restitution = 0.4f; // Bounciness
    
    bool useGravity = true;
    bool isKinematic = false; // Ne bouge pas avec la physique
    
    // AABB Collision box
    glm::vec3 boxMin{-0.5f};
    glm::vec3 boxMax{0.5f};
};

// Contrainte pour relier deux corps (articulations)
struct Constraint {
    RigidBody* bodyA;
    RigidBody* bodyB;
    float restLength;
    float stiffness = 0.8f;
    
    Constraint(RigidBody* a, RigidBody* b, float length)
        : bodyA(a), bodyB(b), restLength(length) {}
};

// Moteur de physique principal
class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();
    
    // Gestion des corps
    RigidBody* CreateRigidBody();
    void RemoveRigidBody(RigidBody* body);
    
    // Gestion des contraintes
    void AddConstraint(RigidBody* a, RigidBody* b, float length);
    
    // Configuration
    void SetGravity(const glm::vec3& gravity) { m_gravity = gravity; }
    glm::vec3 GetGravity() const { return m_gravity; }
    
    // Mise à jour
    void Update(float deltaTime);
    
    // Appliquer des forces
    void ApplyForce(RigidBody* body, const glm::vec3& force);
    void ApplyImpulse(RigidBody* body, const glm::vec3& impulse);
    
    // Détection de collision
    bool CheckCollision(const RigidBody& a, const RigidBody& b);
    void ResolveCollision(RigidBody& a, RigidBody& b);
    
private:
    void IntegrateForces(RigidBody& body, float deltaTime);
    void IntegrateVelocity(RigidBody& body, float deltaTime);
    void SolveConstraints();
    void HandleCollisions();
    
    std::vector<std::unique_ptr<RigidBody>> m_bodies;
    std::vector<Constraint> m_constraints;
    glm::vec3 m_gravity{0.0f, -9.81f, 0.0f};
    
    const int CONSTRAINT_ITERATIONS = 5; // Plus = plus stable
};

} // namespace Engine