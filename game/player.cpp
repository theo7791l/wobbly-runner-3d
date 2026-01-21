#include "player.h"
#include <iostream>

namespace Game {

Player::Player(Engine::PhysicsEngine* physics)
    : m_physics(physics) {
    CreateRagdoll();
}

Player::~Player() {
    // Les bodies sont gérés par le PhysicsEngine
}

void Player::CreateRagdoll() {
    // Tête
    m_head = m_physics->CreateRigidBody();
    m_head->position = m_startPosition + glm::vec3(0.0f, 1.5f, 0.0f);
    m_head->boxMin = glm::vec3(-0.2f, -0.2f, -0.2f);
    m_head->boxMax = glm::vec3(0.2f, 0.2f, 0.2f);
    m_head->mass = 3.0f;
    m_head->restitution = 0.2f;
    
    // Torse
    m_torso = m_physics->CreateRigidBody();
    m_torso->position = m_startPosition + glm::vec3(0.0f, 0.8f, 0.0f);
    m_torso->boxMin = glm::vec3(-0.3f, -0.4f, -0.15f);
    m_torso->boxMax = glm::vec3(0.3f, 0.4f, 0.15f);
    m_torso->mass = 10.0f;
    m_torso->restitution = 0.3f;
    
    // Bassin
    m_pelvis = m_physics->CreateRigidBody();
    m_pelvis->position = m_startPosition + glm::vec3(0.0f, 0.2f, 0.0f);
    m_pelvis->boxMin = glm::vec3(-0.25f, -0.15f, -0.15f);
    m_pelvis->boxMax = glm::vec3(0.25f, 0.15f, 0.15f);
    m_pelvis->mass = 8.0f;
    m_pelvis->restitution = 0.3f;
    
    // Cuisse gauche
    m_leftThigh = m_physics->CreateRigidBody();
    m_leftThigh->position = m_startPosition + glm::vec3(-0.2f, -0.3f, 0.0f);
    m_leftThigh->boxMin = glm::vec3(-0.12f, -0.4f, -0.12f);
    m_leftThigh->boxMax = glm::vec3(0.12f, 0.4f, 0.12f);
    m_leftThigh->mass = 5.0f;
    m_leftThigh->restitution = 0.4f;
    
    // Cuisse droite
    m_rightThigh = m_physics->CreateRigidBody();
    m_rightThigh->position = m_startPosition + glm::vec3(0.2f, -0.3f, 0.0f);
    m_rightThigh->boxMin = glm::vec3(-0.12f, -0.4f, -0.12f);
    m_rightThigh->boxMax = glm::vec3(0.12f, 0.4f, 0.12f);
    m_rightThigh->mass = 5.0f;
    m_rightThigh->restitution = 0.4f;
    
    // Mollet gauche
    m_leftCalf = m_physics->CreateRigidBody();
    m_leftCalf->position = m_startPosition + glm::vec3(-0.2f, -1.1f, 0.0f);
    m_leftCalf->boxMin = glm::vec3(-0.1f, -0.4f, -0.1f);
    m_leftCalf->boxMax = glm::vec3(0.1f, 0.4f, 0.1f);
    m_leftCalf->mass = 3.0f;
    m_leftCalf->restitution = 0.5f;
    m_leftCalf->friction = 0.8f;
    
    // Mollet droit
    m_rightCalf = m_physics->CreateRigidBody();
    m_rightCalf->position = m_startPosition + glm::vec3(0.2f, -1.1f, 0.0f);
    m_rightCalf->boxMin = glm::vec3(-0.1f, -0.4f, -0.1f);
    m_rightCalf->boxMax = glm::vec3(0.1f, 0.4f, 0.1f);
    m_rightCalf->mass = 3.0f;
    m_rightCalf->restitution = 0.5f;
    m_rightCalf->friction = 0.8f;
    
    // Bras gauche
    m_leftArm = m_physics->CreateRigidBody();
    m_leftArm->position = m_startPosition + glm::vec3(-0.5f, 0.6f, 0.0f);
    m_leftArm->boxMin = glm::vec3(-0.1f, -0.4f, -0.1f);
    m_leftArm->boxMax = glm::vec3(0.1f, 0.4f, 0.1f);
    m_leftArm->mass = 2.0f;
    m_leftArm->restitution = 0.3f;
    
    // Bras droit
    m_rightArm = m_physics->CreateRigidBody();
    m_rightArm->position = m_startPosition + glm::vec3(0.5f, 0.6f, 0.0f);
    m_rightArm->boxMin = glm::vec3(-0.1f, -0.4f, -0.1f);
    m_rightArm->boxMax = glm::vec3(0.1f, 0.4f, 0.1f);
    m_rightArm->mass = 2.0f;
    m_rightArm->restitution = 0.3f;
    
    // Stocker toutes les parties
    m_bodyParts = {m_head, m_torso, m_pelvis, m_leftThigh, m_rightThigh,
                   m_leftCalf, m_rightCalf, m_leftArm, m_rightArm};
    
    // Créer les contraintes (articulations)
    m_physics->AddConstraint(m_head, m_torso, 0.4f);         // Cou
    m_physics->AddConstraint(m_torso, m_pelvis, 0.4f);       // Colonne
    m_physics->AddConstraint(m_pelvis, m_leftThigh, 0.3f);   // Hanche gauche
    m_physics->AddConstraint(m_pelvis, m_rightThigh, 0.3f);  // Hanche droite
    m_physics->AddConstraint(m_leftThigh, m_leftCalf, 0.4f); // Genou gauche
    m_physics->AddConstraint(m_rightThigh, m_rightCalf, 0.4f); // Genou droit
    m_physics->AddConstraint(m_torso, m_leftArm, 0.5f);      // Épaule gauche
    m_physics->AddConstraint(m_torso, m_rightArm, 0.5f);     // Épaule droite
}

void Player::LiftLeftLeg() {
    if (m_leftLegCooldown > 0.0f) return;
    
    // Appliquer une force vers le haut et en avant sur la jambe gauche
    glm::vec3 force(0.0f, 400.0f, 150.0f);
    m_physics->ApplyForce(m_leftCalf, force);
    m_physics->ApplyForce(m_leftThigh, force * 0.5f);
    
    // Force vers l'avant sur le bassin pour avancer
    m_physics->ApplyForce(m_pelvis, glm::vec3(0.0f, 0.0f, 80.0f));
    
    m_leftLegCooldown = 0.3f;
    
    std::cout << "🦵 Jambe gauche levée !" << std::endl;
}

void Player::LiftRightLeg() {
    if (m_rightLegCooldown > 0.0f) return;
    
    // Appliquer une force vers le haut et en avant sur la jambe droite
    glm::vec3 force(0.0f, 400.0f, 150.0f);
    m_physics->ApplyForce(m_rightCalf, force);
    m_physics->ApplyForce(m_rightThigh, force * 0.5f);
    
    // Force vers l'avant sur le bassin pour avancer
    m_physics->ApplyForce(m_pelvis, glm::vec3(0.0f, 0.0f, 80.0f));
    
    m_rightLegCooldown = 0.3f;
    
    std::cout << "🦵 Jambe droite levée !" << std::endl;
}

void Player::LeanForward() {
    // Pencher le torse vers l'avant
    glm::vec3 force(0.0f, -50.0f, 100.0f);
    m_physics->ApplyForce(m_torso, force);
    m_physics->ApplyForce(m_head, force * 0.5f);
}

void Player::LeanBackward() {
    // Pencher le torse vers l'arrière
    glm::vec3 force(0.0f, -50.0f, -100.0f);
    m_physics->ApplyForce(m_torso, force);
    m_physics->ApplyForce(m_head, force * 0.5f);
}

void Player::Jump() {
    if (m_jumpCooldown > 0.0f) return;
    
    // Vérifier si on est au sol
    bool onGround = false;
    for (auto* part : m_bodyParts) {
        if (part->position.y < 1.0f) {
            onGround = true;
            break;
        }
    }
    
    if (onGround) {
        // Appliquer une impulsion vers le haut sur toutes les parties
        for (auto* part : m_bodyParts) {
            m_physics->ApplyImpulse(part, glm::vec3(0.0f, 150.0f, 0.0f));
        }
        m_jumpCooldown = 1.0f;
        std::cout << "🚀 SAUT !" << std::endl;
    }
}

void Player::Update(float deltaTime) {
    // Décrémenter les cooldowns
    if (m_leftLegCooldown > 0.0f) m_leftLegCooldown -= deltaTime;
    if (m_rightLegCooldown > 0.0f) m_rightLegCooldown -= deltaTime;
    if (m_jumpCooldown > 0.0f) m_jumpCooldown -= deltaTime;
    
    // Vérifier si le joueur est tombé trop bas
    glm::vec3 pos = GetPosition();
    if (pos.y < -10.0f) {
        std::cout << "⚠️  Tu es tombé ! Recommence avec R" << std::endl;
    }
}

void Player::Render(Engine::Renderer* renderer) {
    // Couleurs pour les différentes parties
    glm::vec3 headColor(1.0f, 0.8f, 0.7f);    // Beige
    glm::vec3 torsoColor(0.2f, 0.4f, 0.8f);   // Bleu
    glm::vec3 limbColor(0.3f, 0.5f, 0.9f);    // Bleu clair
    glm::vec3 legColor(0.15f, 0.3f, 0.6f);    // Bleu foncé
    
    // Dessiner chaque partie du corps
    renderer->DrawSphere(m_head->position, 0.4f, headColor);
    
    renderer->DrawCube(m_torso->position,
                      m_torso->boxMax - m_torso->boxMin,
                      torsoColor);
    
    renderer->DrawCube(m_pelvis->position,
                      m_pelvis->boxMax - m_pelvis->boxMin,
                      torsoColor);
    
    renderer->DrawCube(m_leftThigh->position,
                      m_leftThigh->boxMax - m_leftThigh->boxMin,
                      legColor);
    
    renderer->DrawCube(m_rightThigh->position,
                      m_rightThigh->boxMax - m_rightThigh->boxMin,
                      legColor);
    
    renderer->DrawCube(m_leftCalf->position,
                      m_leftCalf->boxMax - m_leftCalf->boxMin,
                      limbColor);
    
    renderer->DrawCube(m_rightCalf->position,
                      m_rightCalf->boxMax - m_rightCalf->boxMin,
                      limbColor);
    
    renderer->DrawCube(m_leftArm->position,
                      m_leftArm->boxMax - m_leftArm->boxMin,
                      limbColor);
    
    renderer->DrawCube(m_rightArm->position,
                      m_rightArm->boxMax - m_rightArm->boxMin,
                      limbColor);
    
    // Dessiner les articulations (lignes)
    glm::vec3 jointColor(0.9f, 0.9f, 0.1f);
    renderer->DrawLine(m_head->position, m_torso->position, jointColor);
    renderer->DrawLine(m_torso->position, m_pelvis->position, jointColor);
    renderer->DrawLine(m_pelvis->position, m_leftThigh->position, jointColor);
    renderer->DrawLine(m_pelvis->position, m_rightThigh->position, jointColor);
    renderer->DrawLine(m_leftThigh->position, m_leftCalf->position, jointColor);
    renderer->DrawLine(m_rightThigh->position, m_rightCalf->position, jointColor);
    renderer->DrawLine(m_torso->position, m_leftArm->position, jointColor);
    renderer->DrawLine(m_torso->position, m_rightArm->position, jointColor);
}

glm::vec3 Player::GetPosition() const {
    // Position moyenne du corps (centre de masse approximatif)
    glm::vec3 sum(0.0f);
    for (auto* part : m_bodyParts) {
        sum += part->position;
    }
    return sum / static_cast<float>(m_bodyParts.size());
}

void Player::Reset() {
    // Réinitialiser toutes les parties du corps
    float yOffset = 1.5f;
    m_head->position = m_startPosition + glm::vec3(0.0f, yOffset, 0.0f);
    m_torso->position = m_startPosition + glm::vec3(0.0f, yOffset - 0.7f, 0.0f);
    m_pelvis->position = m_startPosition + glm::vec3(0.0f, yOffset - 1.3f, 0.0f);
    m_leftThigh->position = m_startPosition + glm::vec3(-0.2f, yOffset - 1.8f, 0.0f);
    m_rightThigh->position = m_startPosition + glm::vec3(0.2f, yOffset - 1.8f, 0.0f);
    m_leftCalf->position = m_startPosition + glm::vec3(-0.2f, yOffset - 2.6f, 0.0f);
    m_rightCalf->position = m_startPosition + glm::vec3(0.2f, yOffset - 2.6f, 0.0f);
    m_leftArm->position = m_startPosition + glm::vec3(-0.5f, yOffset - 0.9f, 0.0f);
    m_rightArm->position = m_startPosition + glm::vec3(0.5f, yOffset - 0.9f, 0.0f);
    
    // Reset des vélocités
    for (auto* part : m_bodyParts) {
        part->velocity = glm::vec3(0.0f);
        part->force = glm::vec3(0.0f);
    }
    
    // Reset des cooldowns
    m_leftLegCooldown = 0.0f;
    m_rightLegCooldown = 0.0f;
    m_jumpCooldown = 0.0f;
}

} // namespace Game