# 🏛️ Architecture du Moteur - Wobbly Runner 3D

Ce document explique l'architecture du moteur de jeu et comment les différents composants interagissent.

## 📋 Vue d'ensemble

```
┌────────────────────────┐
│   WOBBLY RUNNER 3D      │
│      (main.cpp)         │
└────────┬───────────────┘
         │
    ┌────┼────┐
    │         │
┌───v───┐ ┌──v────┐
│ ENGINE │ │  GAME  │
└───┬───┘ └──┬────┘
    │         │
┌───v───┐ ┌──v────┐
│Physics│ │Player │
│Renderer│ │Level  │
│ Input  │ └───────┘
└───────┘
```

## 📦 Modules

### 1. Engine (Moteur de base)

Le moteur fournit les services fondamentaux pour le jeu.

#### **PhysicsEngine** (`engine/physics.*`)

**Responsabilités:**
- Simulation de corps rigides
- Intégration physique (Verlet)
- Détection et résolution de collisions
- Système de contraintes (articulations)
- Gestion de la gravité et des forces

**Composants clés:**
```cpp
struct RigidBody {
    glm::vec3 position, velocity, acceleration;
    float mass, friction, restitution;
    bool useGravity, isKinematic;
};

struct Constraint {
    RigidBody *bodyA, *bodyB;
    float restLength, stiffness;
};
```

**Algorithme principal:**
1. **Intégration des forces** : F = ma
2. **Résolution des contraintes** : Maintenir les distances entre corps
3. **Intégration des vélocités** : position += velocity * dt
4. **Collisions** : Détection AABB + résolution par impulsion

#### **Renderer** (`engine/renderer.*`)

**Responsabilités:**
- Initialisation OpenGL/GLFW
- Gestion de la fenêtre
- Rendu de primitives (cubes, sphères, lignes)
- Système de caméra

**Architecture:**
- Mode immediate (simplifié pour le début)
- Caméra lookAt classique
- Projection perspective

**Améliorations possibles:**
- Passer à un système de VAO/VBO moderne
- Ajouter des shaders custom
- Implémenter un système de mesh

#### **InputSystem** (`engine/input.*`)

**Responsabilités:**
- Polling des entrées clavier
- Détection des événements (press, hold, release)
- Support souris (préparé)

**Pattern:**
- Observer pattern pour les inputs
- État précédent/actuel pour détecter les changements

### 2. Game (Logique du jeu)

#### **Player** (`game/player.*`)

**Responsabilités:**
- Création et gestion du ragdoll
- Traitement des commandes (Q/D/Z/S/Espace)
- Rendu du personnage

**Architecture Ragdoll:**
```
     [Tête]
       |
    [Torse]
    /  |  \
 [BA] | [BD]  (Bras)
      |
   [Bassin]
    /    \
 [CG]    [CD]  (Cuisses)
  |       |
 [MG]    [MD]  (Mollets)
```

**Physique:**
- 9 corps rigides connectés
- 8 contraintes (articulations)
- Forces appliquées pour les mouvements
- Système de cooldown pour éviter le spam

**Mouvements:**
- **LiftLeg**: Force verticale + avant sur la jambe
- **Lean**: Force sur le torse
- **Jump**: Impulsion sur tous les corps

#### **Level** (`game/level.*`)

**Responsabilités:**
- Génération procédurale du parcours
- Gestion des obstacles
- Animation des obstacles dynamiques

**Types d'obstacles:**
1. **Platform**: Plateforme statique
2. **RotatingBar**: Barre rotative (danger)
3. **MovingPlatform**: Plateforme oscillante
4. **Gap**: Trou à sauter
5. **Ramp**: Rampe pour prendre de la hauteur

**Génération:**
- Aléatoire avec seed
- Espacement variable
- Ligne d'arrivée à la fin

## 🔄 Boucle de jeu

```cpp
while (!renderer.ShouldClose()) {
    // 1. INPUT
    inputSystem.Update();
    ProcessPlayerCommands();
    
    // 2. UPDATE
    physics.Update(deltaTime);
    player.Update(deltaTime);
    level.Update(deltaTime);
    
    // 3. RENDER
    renderer.BeginFrame();
    UpdateCamera();
    level.Render();
    player.Render();
    renderer.EndFrame();
}
```

## 🔌 Diagramme de flux

```
     INPUT
       ↓
    PLAYER
       ↓ (apply forces)
    PHYSICS
       ↓ (integrate)
 RIGID BODIES
       ↓ (positions)
    RENDERER
       ↓
     SCREEN
```

## ⚡ Optimisations possibles

### Performance
1. **Spatial partitioning** pour les collisions (Octree, BVH)
2. **Fixed timestep** pour la physique
3. **Object pooling** pour les obstacles
4. **Instanced rendering** pour les primitives

### Features
1. **Shaders avancés** (lighting, shadows)
2. **Particules** pour les effets visuels
3. **Son** avec OpenAL ou SDL_mixer
4. **Menu UI** avec ImGui
5. **Sauvegarde** des highscores
6. **Replay system**

## 🎯 Design Patterns utilisés

1. **Component Pattern**: RigidBody, Constraint
2. **Factory Pattern**: CreateRigidBody()
3. **Update Pattern**: Update(deltaTime)
4. **Singleton-ish**: PhysicsEngine, Renderer (un par jeu)

## 📐 Structure des données

### Mémoire
- Bodies stockés dans `std::vector<unique_ptr<RigidBody>>`
- Contraintes dans `std::vector<Constraint>`
- Ownership claire avec smart pointers

### Performance
- Cache-friendly avec structures contiguës
- Itération séquentielle sur les bodies
- Pas d'allocation durant le gameplay

## 📚 Références

- **Physics**: [Game Physics Engine Development](https://www.goodreads.com/book/show/187726.Game_Physics_Engine_Development)
- **Ragdoll**: [Jakobsen's Method](http://www.cs.cmu.edu/afs/cs/academic/class/15462-s13/www/lec_slides/Jakobsen.pdf)
- **Rendering**: [LearnOpenGL](https://learnopengl.com/)
- **Architecture**: [Game Programming Patterns](https://gameprogrammingpatterns.com/)

---

Cette architecture est volontairement simple pour faciliter l'apprentissage. Chaque composant peut être étendu indépendamment !