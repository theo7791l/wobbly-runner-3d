#include <iostream>
#include <memory>
#include "engine/physics.h"
#include "engine/renderer.h"
#include "engine/input.h"
#include "game/player.h"
#include "game/level.h"

// Configuration de la fenêtre
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "Wobbly Runner 3D - Atteins la ligne d'arrivée !";

int main() {
    std::cout << "=================================" << std::endl;
    std::cout << "  🎮 WOBBLY RUNNER 3D 🎮  " << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << "Objectif: Traverse le parcours avec ton personnage ragdoll !" << std::endl;
    std::cout << "\nContrôles:" << std::endl;
    std::cout << "  Q/D - Jambes gauche/droite" << std::endl;
    std::cout << "  Z/S - Pencher avant/arrière" << std::endl;
    std::cout << "  ESPACE - Sauter" << std::endl;
    std::cout << "  R - Recommencer" << std::endl;
    std::cout << "  ESC - Quitter" << std::endl;
    std::cout << "=================================\n" << std::endl;

    try {
        // Initialisation du renderer
        auto renderer = std::make_unique<Engine::Renderer>();
        if (!renderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)) {
            std::cerr << "❌ Erreur: Impossible d'initialiser le renderer" << std::endl;
            return -1;
        }

        // Initialisation du système d'input
        auto inputSystem = std::make_unique<Engine::InputSystem>(renderer->GetWindow());

        // Initialisation du moteur de physique
        auto physics = std::make_unique<Engine::PhysicsEngine>();
        physics->SetGravity({0.0f, -9.81f, 0.0f});

        // Création du joueur
        auto player = std::make_unique<Game::Player>(physics.get());

        // Création du niveau
        auto level = std::make_unique<Game::Level>(physics.get());
        level->GenerateObstacleCourse(50.0f); // Parcours de 50m

        // Variables de temps
        float lastTime = 0.0f;
        float deltaTime = 0.0f;
        
        bool gameWon = false;
        float gameTime = 0.0f;

        std::cout << "✅ Jeu initialisé ! Bonne chance !\n" << std::endl;

        // Boucle de jeu principale
        while (!renderer->ShouldClose()) {
            // Calcul du temps
            float currentTime = renderer->GetTime();
            deltaTime = currentTime - lastTime;
            lastTime = currentTime;
            gameTime += deltaTime;

            // Input
            inputSystem->Update();
            
            // Commandes du joueur
            if (inputSystem->IsKeyPressed(GLFW_KEY_Q)) {
                player->LiftLeftLeg();
            }
            if (inputSystem->IsKeyPressed(GLFW_KEY_D)) {
                player->LiftRightLeg();
            }
            if (inputSystem->IsKeyPressed(GLFW_KEY_Z)) {
                player->LeanForward();
            }
            if (inputSystem->IsKeyPressed(GLFW_KEY_S)) {
                player->LeanBackward();
            }
            if (inputSystem->IsKeyPressed(GLFW_KEY_SPACE)) {
                player->Jump();
            }
            if (inputSystem->IsKeyPressed(GLFW_KEY_R)) {
                player->Reset();
                level->Reset();
                gameWon = false;
                gameTime = 0.0f;
                std::cout << "🔄 Niveau recommencé !" << std::endl;
            }

            // Mise à jour physique
            physics->Update(deltaTime);
            player->Update(deltaTime);
            level->Update(deltaTime);

            // Vérification de la victoire
            if (!gameWon && player->GetPosition().z >= 50.0f) {
                gameWon = true;
                std::cout << "\n🎉🎉🎉 VICTOIRE ! 🎉🎉🎉" << std::endl;
                std::cout << "Temps: " << static_cast<int>(gameTime) << " secondes" << std::endl;
                std::cout << "Tu as survécu au parcours de Wobby !\n" << std::endl;
            }

            // Rendu
            renderer->BeginFrame();
            
            // Caméra qui suit le joueur
            glm::vec3 playerPos = player->GetPosition();
            renderer->SetCameraPosition(playerPos + glm::vec3(0.0f, 5.0f, -10.0f));
            renderer->SetCameraTarget(playerPos);

            // Rendu du niveau
            level->Render(renderer.get());
            
            // Rendu du joueur
            player->Render(renderer.get());

            renderer->EndFrame();
        }

        std::cout << "\n👋 Merci d'avoir joué à Wobbly Runner 3D !" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Erreur fatale: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}