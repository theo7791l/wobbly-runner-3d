# 🛠️ Guide d'Installation - Wobbly Runner 3D

Ce guide t'aidera à compiler et exécuter Wobbly Runner 3D sur ta machine.

## 💻 Prérequis

### Tous les systèmes
- **CMake** 3.15 ou supérieur
- **Compilateur C++17** (GCC 7+, Clang 5+, MSVC 2017+)
- **Git**

### Dépendances externes
- **GLFW** 3.3+ (fenêtre et input)
- **GLAD** (OpenGL loader)
- **GLM** (mathématiques 3D)

---

## 🐧 Linux (Ubuntu/Debian)

### 1. Installer les outils de développement

```bash
sudo apt update
sudo apt install build-essential cmake git
```

### 2. Installer les dépendances

```bash
# GLFW et GLM
sudo apt install libglfw3-dev libglm-dev

# Pour GLAD, télécharger depuis https://glad.dav1d.de/
# Ou utiliser le script ci-dessous
```

### 3. Cloner et compiler

```bash
git clone https://github.com/theo7791l/wobbly-runner-3d.git
cd wobbly-runner-3d

# Créer le dossier build
mkdir build && cd build

# Générer avec CMake
cmake ..

# Compiler
make -j$(nproc)

# Exécuter
./WobblyRunner
```

---

## 🍎 macOS

### 1. Installer Homebrew (si pas déjà installé)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Installer les outils et dépendances

```bash
brew install cmake glfw glm
```

### 3. Cloner et compiler

```bash
git clone https://github.com/theo7791l/wobbly-runner-3d.git
cd wobbly-runner-3d

mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

./WobblyRunner
```

---

## 🪟 Windows

### Méthode 1: Avec vcpkg (Recommandé)

#### 1. Installer Visual Studio

Télécharge et installe [Visual Studio 2019/2022](https://visualstudio.microsoft.com/) avec "Desktop development with C++".

#### 2. Installer vcpkg

```powershell
# Dans PowerShell
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Intégrer vcpkg avec Visual Studio
.\vcpkg integrate install
```

#### 3. Installer les dépendances

```powershell
.\vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows
```

#### 4. Compiler le projet

```powershell
git clone https://github.com/theo7791l/wobbly-runner-3d.git
cd wobbly-runner-3d

mkdir build
cd build

# Générer avec vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

# Compiler
cmake --build . --config Release

# Exécuter
.\Release\WobblyRunner.exe
```

### Méthode 2: Installation manuelle

1. Télécharge les bibliothèques:
   - [GLFW](https://www.glfw.org/download.html) (Windows pre-compiled)
   - [GLM](https://github.com/g-truc/glm/releases)
   - [GLAD](https://glad.dav1d.de/) (génère avec OpenGL 3.3+)

2. Place-les dans un dossier `external/` à la racine du projet

3. Modifie le `CMakeLists.txt` pour pointer vers ces dossiers

---

## 🔧 Configuration GLAD

GLAD doit être généré avec les paramètres suivants:

- **API**: OpenGL
- **Version**: 3.3+
- **Profile**: Core
- **Générer un loader**: Oui

Visite [https://glad.dav1d.de/](https://glad.dav1d.de/) et télécharge `glad.c` et `glad.h`.

Place-les dans:
```
wobbly-runner-3d/
├── external/
│   └── glad/
│       ├── include/
│       │   └── glad/
│       │       └── glad.h
│       └── src/
│           └── glad.c
```

---

## ⚠️ Dépannage

### Erreur: "GLFW not found"

**Linux**: `sudo apt install libglfw3-dev`

**macOS**: `brew install glfw`

**Windows**: Vérifie que vcpkg a installé GLFW correctement

### Erreur: "OpenGL functions not loaded"

Vérifie que GLAD est bien inclus et initialisé après la création du contexte OpenGL.

### Le jeu ne se lance pas

Assure-toi que ta carte graphique supporte OpenGL 3.3+:

```bash
# Linux
glxinfo | grep "OpenGL version"

# macOS
system_profiler SPDisplaysDataType
```

### Problèmes de performance

- Active VSync dans le renderer (déjà activé par défaut)
- Vérifie que tu utilises la carte graphique dédiée (laptops)
- Réduis la résolution de la fenêtre dans `main.cpp`

---

## 🎮 Test rapide

Après compilation, tu devrais voir:

```
=================================
  🎮 WOBBLY RUNNER 3D 🎮
=================================
Objectif: Traverse le parcours avec ton personnage ragdoll !

Contrôles:
  Q/D - Jambes gauche/droite
  Z/S - Pencher avant/arrière
  ESPACE - Sauter
  R - Recommencer
  ESC - Quitter
=================================

✅ Renderer initialisé (OpenGL 3.3.0)
🏭 Génération du parcours d'obstacles...
✅ Parcours généré : 12 obstacles sur 50m
✅ Jeu initialisé ! Bonne chance !
```

---

## 📚 Ressources supplémentaires

- [Documentation GLFW](https://www.glfw.org/documentation.html)
- [Documentation GLM](https://github.com/g-truc/glm/blob/master/manual.md)
- [OpenGL Tutorial](https://learnopengl.com/)

---

## 🐛 Besoin d'aide?

Ouvre une issue sur GitHub si tu rencontres des problèmes !

**Bon courage pour atteindre la ligne d'arrivée ! 🏁**