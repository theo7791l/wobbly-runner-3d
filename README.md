# 🎮 Wobbly Runner 3D

**Un jeu de parcours d'obstacles hilarant en 3D avec physique ragdoll custom !**

## 🎯 Objectif du Jeu

Tu incarnes "Wobby", un personnage maladroit aux articulations élastiques qui doit traverser un parcours d'obstacles délirants. Le problème ? Chaque pas que tu fais est imprévisible et hilarant !

### Règles
- Utilise **Q/D** pour les jambes gauche/droite
- Utilise **Z/S** pour te pencher avant/arrière
- Atteins la ligne d'arrivée sans tomber (trop souvent)
- Plus tu te déplaces vite, plus c'est drôle (et difficile) !

## 🛠️ Architecture du Moteur

Ce projet est un **moteur de jeu 3D complet codé from scratch en C++** avec :

### Composants Principaux

- **Physics Engine** (`engine/physics.*`)
  - Système de corps rigides avec intégration de Verlet
  - Détection de collisions (AABB et sphères)
  - Contraintes pour les articulations ragdoll
  - Gravité et forces customisables

- **Renderer** (`engine/renderer.*`)
  - Rendu OpenGL moderne (3.3+)
  - Système de caméra 3D
  - Rendu de primitives (cubes, sphères)
  - Gestion des shaders

- **Input System** (`engine/input.*`)
  - Gestion clavier via GLFW
  - Mappings personnalisables

- **Game Logic** (`game/*`)
  - Système de personnage ragdoll
  - Générateur de niveaux procédural
  - Logique d'obstacles

## 📦 Dépendances

- **GLFW** : Gestion fenêtre et input
- **GLEW** : Chargement OpenGL
- **GLM** : Mathématiques 3D
- **C++17** minimum

## 🚀 Compilation

### Installation des dépendances

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libglew-dev libglm-dev

# Fedora/RHEL
sudo dnf install cmake gcc-c++ glfw-devel glew-devel glm-devel

# Arch Linux
sudo pacman -S cmake glfw-x11 glew glm

# macOS
brew install cmake glfw glew glm
```

### Compilation

```bash
# Clone le repo
git clone https://github.com/theo7791l/wobbly-runner-3d.git
cd wobbly-runner-3d

# Compile
mkdir build && cd build
cmake ..
make -j$(nproc)

# Lance le jeu
./WobblyRunner
```

### Windows (Visual Studio)

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
.\Release\WobblyRunner.exe
```

## 🎮 Contrôles

| Touche | Action |
|--------|--------|
| **Q** | Lever jambe gauche |
| **D** | Lever jambe droite |
| **Z** | Pencher avant |
| **S** | Pencher arrière |
| **Espace** | Sauter (si tu oses) |
| **R** | Recommencer |
| **Échap** | Quitter |

## 🎨 Features

- ✅ Moteur de physique 3D custom
- ✅ Système ragdoll hilarant
- ✅ Parcours procédural
- ✅ Physique exagérée pour le fun
- ✅ Caméra qui suit le personnage
- ✅ Obstacles variés (plateformes mouvantes, barres rotatives, etc.)

## 🔧 Structure du Code

```
wobbly-runner-3d/
├── CMakeLists.txt          # Configuration build
├── main.cpp                # Point d'entrée
├── engine/
│   ├── physics.h/cpp       # Moteur physique
│   ├── renderer.h/cpp      # Système de rendu
│   └── input.h/cpp         # Gestion input
└── game/
    ├── player.h/cpp        # Personnage ragdoll
    └── level.h/cpp         # Génération niveau
```

## 🎓 Apprendre de ce Projet

Ce projet est parfait pour comprendre :
- La physique 3D en temps réel
- L'architecture d'un moteur de jeu
- OpenGL et rendu 3D
- Les systèmes ragdoll
- La génération procédurale

## 📝 License

MIT License - Fais ce que tu veux avec !

## 🤝 Contribution

N'hésite pas à ajouter des obstacles délirants, améliorer la physique, ou rendre le jeu encore plus drôle !

---

**Bon courage pour atteindre la ligne d'arrivée ! 🏁**
