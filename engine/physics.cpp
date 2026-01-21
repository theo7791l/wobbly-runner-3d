#include "physics.h"
#include <algorithm>
#include <cmath>

namespace Engine {

PhysicsEngine::PhysicsEngine() {}

PhysicsEngine::~PhysicsEngine() {}

RigidBody* PhysicsEngine::CreateRigidBody() {
    auto body = std::make_unique<RigidBody>();
    RigidBody* ptr = body.get();
    m_bodies.push_back(std::move(body));
    return ptr;
}

void PhysicsEngine::RemoveRigidBody(RigidBody* body) {
    m_bodies.erase(
        std::remove_if(m_bodies.begin(), m_bodies.end(),
            [body](const auto& b) { return b.get() == body; }),
        m_bodies.end()
    );
}

void PhysicsEngine::AddConstraint(RigidBody* a, RigidBody* b, float length) {
    m_constraints.emplace_back(a, b, length);
}

void PhysicsEngine::ApplyForce(RigidBody* body, const glm::vec3& force) {
    if (!body || body->isKinematic) return;
    body->force += force;
}

void PhysicsEngine::ApplyImpulse(RigidBody* body, const glm::vec3& impulse) {
    if (!body || body->isKinematic) return;
    body->velocity += impulse / body->mass;
}

void PhysicsEngine::Update(float deltaTime) {
    // Limiter le pas de temps pour la stabilité
    const float maxDelta = 0.02f;
    deltaTime = std::min(deltaTime, maxDelta);
    
    // Intégration des forces
    for (auto& body : m_bodies) {
        if (!body->isKinematic) {
            IntegrateForces(*body, deltaTime);
        }
    }
    
    // Résoudre les contraintes (articulations)
    for (int i = 0; i < CONSTRAINT_ITERATIONS; ++i) {
        SolveConstraints();
    }
    
    // Intégration des vélocités
    for (auto& body : m_bodies) {
        if (!body->isKinematic) {
            IntegrateVelocity(*body, deltaTime);
        }
    }
    
    // Collisions
    HandleCollisions();
    
    // Reset des forces
    for (auto& body : m_bodies) {
        body->force = glm::vec3(0.0f);
    }
}

void PhysicsEngine::IntegrateForces(RigidBody& body, float deltaTime) {
    if (body.mass <= 0.0f) return;
    
    // Ajouter la gravité
    if (body.useGravity) {
        body.force += m_gravity * body.mass;
    }
    
    // Calculer l'accélération (F = ma)
    body.acceleration = body.force / body.mass;
    
    // Mettre à jour la vélocité
    body.velocity += body.acceleration * deltaTime;
    
    // Friction aérienne simple
    body.velocity *= 0.995f;
}

void PhysicsEngine::IntegrateVelocity(RigidBody& body, float deltaTime) {
    body.position += body.velocity * deltaTime;
    
    // Friction au sol (simple)
    if (body.position.y <= body.boxMin.y + 0.01f) {
        body.velocity.x *= (1.0f - body.friction * deltaTime * 10.0f);
        body.velocity.z *= (1.0f - body.friction * deltaTime * 10.0f);
    }
}

void PhysicsEngine::SolveConstraints() {
    for (auto& constraint : m_constraints) {
        if (!constraint.bodyA || !constraint.bodyB) continue;
        
        // Calculer la différence de position
        glm::vec3 delta = constraint.bodyB->position - constraint.bodyA->position;
        float distance = glm::length(delta);
        
        if (distance < 0.0001f) continue;
        
        // Calculer la correction
        float error = distance - constraint.restLength;
        glm::vec3 correction = (delta / distance) * error * constraint.stiffness;
        
        // Appliquer la correction (50/50 si les deux bougent)
        if (!constraint.bodyA->isKinematic && !constraint.bodyB->isKinematic) {
            constraint.bodyA->position += correction * 0.5f;
            constraint.bodyB->position -= correction * 0.5f;
        } else if (!constraint.bodyA->isKinematic) {
            constraint.bodyA->position += correction;
        } else if (!constraint.bodyB->isKinematic) {
            constraint.bodyB->position -= correction;
        }
    }
}

bool PhysicsEngine::CheckCollision(const RigidBody& a, const RigidBody& b) {
    glm::vec3 aMin = a.position + a.boxMin;
    glm::vec3 aMax = a.position + a.boxMax;
    glm::vec3 bMin = b.position + b.boxMin;
    glm::vec3 bMax = b.position + b.boxMax;
    
    return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
           (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
           (aMin.z <= bMax.z && aMax.z >= bMin.z);
}

void PhysicsEngine::ResolveCollision(RigidBody& a, RigidBody& b) {
    // Calculer la normale de collision
    glm::vec3 normal = glm::normalize(b.position - a.position);
    
    // Vélocité relative
    glm::vec3 relativeVel = b.velocity - a.velocity;
    float velAlongNormal = glm::dot(relativeVel, normal);
    
    // Ne pas résoudre si les objets s'éloignent
    if (velAlongNormal > 0) return;
    
    // Calculer l'impulsion
    float restitution = std::min(a.restitution, b.restitution);
    float impulseMagnitude = -(1.0f + restitution) * velAlongNormal;
    impulseMagnitude /= (1.0f / a.mass + 1.0f / b.mass);
    
    glm::vec3 impulse = impulseMagnitude * normal;
    
    // Appliquer l'impulsion
    if (!a.isKinematic) a.velocity -= impulse / a.mass;
    if (!b.isKinematic) b.velocity += impulse / b.mass;
}

void PhysicsEngine::HandleCollisions() {
    // Collision simple avec le sol
    for (auto& body : m_bodies) {
        if (body->isKinematic) continue;
        
        float groundY = 0.0f;
        float bodyBottom = body->position.y + body->boxMin.y;
        
        if (bodyBottom < groundY) {
            body->position.y = groundY - body->boxMin.y;
            
            // Rebond
            if (body->velocity.y < 0) {
                body->velocity.y *= -body->restitution;
                
                // Arrêter de rebondir si trop lent
                if (std::abs(body->velocity.y) < 0.1f) {
                    body->velocity.y = 0.0f;
                }
            }
        }
    }
    
    // Collisions entre corps
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        for (size_t j = i + 1; j < m_bodies.size(); ++j) {
            if (CheckCollision(*m_bodies[i], *m_bodies[j])) {
                ResolveCollision(*m_bodies[i], *m_bodies[j]);
            }
        }
    }
}

} // namespace Engine